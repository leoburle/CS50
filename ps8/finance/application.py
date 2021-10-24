import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import *

from datetime import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user_id = session["user_id"]

    numberOfDecPlaces = 2

    wallet = getUserWallet(user_id)
    wallet_updated = wallet


    remaining_cash = round(getAvailableCash(user_id), numberOfDecPlaces)

    # Check if the user has at least one stock
    if wallet:
        # Update the users wallet data
        # consulting the current price each stock
        count = 0
        for trade in wallet:

            symbol = trade["stock"]
            shares = int(trade["shares"])
            price = float(lookup(symbol)["price"])

            wallet_updated[count].update({"price": price})
            wallet_updated[count].update({"total": shares*price})

            count += 1
    else:
        pass

    return render_template("index.html", wallet = wallet_updated, cash = remaining_cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """buy a number o shares of a stock"""

    if request.method == "POST":

        stock = request.form.get("symbol")
        shares = request.form.get("shares")
        user_id = session["user_id"]
        typeOfTrade = 'buy'

        stock_data = lookup(stock)

        print("shares "+ shares)

        stock = stock_data

        if stock_data:
            price = round(float(stock_data["price"]), 2)
        else:
            return apology("Stock not found!")

        if isSharesValid(shares):
            shares = int(shares)
        else:
            return apology("Invalid number of shares!")


        cash = getAvailableCash(user_id)

        if  canUserBuy(cash, shares, price):

            name = stock_data["name"]

            updateCash(user_id, shares, price)
            updatePortfolio(user_id, stock, shares)
            registerTrade(stock, shares, price, user_id, typeOfTrade)

            remaining_cash = getAvailableCash(user_id)

            return render_template("/bought.html", symbol=stock, name=name,
                                    shares=shares, price=price, total=price*shares, cash=remaining_cash)

        else:

            return apology("You don´t have enough money!")

    else:

        return render_template("/buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    user_id = session["user_id"]

    if request.method == 'GET':

        history_data = getUserHistory(user_id)

        return render_template("history.html", history = history_data)



@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    if request.method == 'POST':

        symbol = request.form.get("symbol")
        stock_data = lookup(symbol)

        name = stock_data["name"]
        price = stock_data["price"]
        symbol = stock_data["symbol"]

        return render_template("quoted.html", name = name, price = price, symbol = symbol)

    else:

        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("signIn.html")
    else:
        password = request.form.get("password")
        repeatedPassword = request.form.get("repeated-password")

        if isPasswordValid(password):

            if password == repeatedPassword:
                passwordHash = generate_password_hash(password)
                username = request.form.get("username")
                db.execute("INSERT INTO users (username, hash)"
                          +"VALUES (?, ?)", username, passwordHash)
                return redirect("/login")

            else:
                return apology("The passwords doesn't match. Try again!")

        else:
            return apology("Invalid password! The password must contain "
                          +"at least 6 characters, containing at least a letter and a number")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    typeOfTrade = 'sell';

    if request.method == "GET":

        wallet = getUserWallet(user_id)

        return render_template("sell.html", wallet=wallet)

    else:

        stock = request.form.get("symbol")
        shares = request.form.get("shares")

        if isSharesValid(shares):
            shares = int(shares)
        else:
            return apology("Invalid number of shares")

        if not(canUserSell(user_id, stock, shares)):
            return apology("You are trying to sell more shares than you have")

        # number of shares to sell
        # Reuse functions of buy
        shares = shares * -1

        price = lookup(stock)["price"]

        updatePortfolio(user_id, stock, shares)
        updateCash(user_id, shares, price)
        registerTrade(stock, shares, price, user_id, typeOfTrade)

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

def isPasswordValid(password):

    if hasDigit(password) and hasChar(password) and hasProperLength(password):
        return True
    else:
        return False

# Checks if the string has one ore more digits
def hasDigit(inputString):

    for symbol in inputString:
        if symbol.isdigit():
            return True

    return False


# Checks if the string has one ore more letter
def hasChar(inputString):

    for symbol in inputString:
        if symbol.isalpha():
            return True

    return False

# Check if the string has more than 6 chars
def hasProperLength(inputString):

    if len(inputString) < 6:
        return False

    return True

# get users available cash
def getAvailableCash(user_id):

    cash = db.execute("SELECT cash FROM "+
                  "users WHERE id = ?", user_id)

    return float(cash[0]["cash"])

# checks if the user has enough money to do a transaction
def canUserBuy(cash, shares, price):

    shares = int(shares)
    price = float(price)

    if cash >= shares * price:
        return True
    else:
        return False

# Updates the cash register from user
def updateCash(user_id, shares, price):

    cash = getAvailableCash(user_id)
    totalPrice = shares * price

    leftCash = cash - totalPrice

    db.execute("UPDATE users SET cash = ?", leftCash)

# Updates user portfolio
def updatePortfolio(user_id, stock, traded_shares):

    shares = db.execute("SELECT shares FROM portfolio WHERE stock = ? and owner_id = ?", stock, user_id)

    # Checks if the user already has an stock of that company and
    # and updates the table
    if shares:

        # computates the resulting number of shares
        # increased or decreased
        shares = shares[0]["shares"] + traded_shares

        if shares == 0:
            db.execute("DELETE FROM portfolio WHERE"
                    " owner_id = ? AND stock = ?", user_id, stock)
        else:
            db.execute("UPDATE portfolio SET shares = ? WHERE"
                    " owner_id = ? AND stock = ?", shares, user_id, stock)
    else:

        db.execute("INSERT INTO portfolio (stock, shares, owner_id) VALUES (?, ?, ?)",
                   stock, traded_shares, user_id)


def registerTrade(stock, shares, price, user_id, typeOfTrade):

    stock = stock.upper()

    shares = int(shares)

    time = datetime.today().strftime('%Y-%m-%d-%H:%M:%S')

    db.execute("INSERT INTO transactions (stock, shares, price, time,"
              +"buyer_id, type) VALUES (?, ?, ? ,?, ?, ?)",
              stock, shares, price, time, user_id, typeOfTrade)



def getUserWallet(user_id):

        return db.execute("SELECT * FROM"
                         +" portfolio WHERE owner_id = ?", user_id)



def canUserSell(user_id, symbol, sharesToSell):

    shares = db.execute("SELECT shares FROM portfolio WHERE "+
                    "owner_id = ? and stock = ?", user_id, symbol)

    if shares[0]["shares"] >= sharesToSell:

        return True

    else:

        return False

# Checks if the inputed number of shares is valid
def isSharesValid(shares):

    try:
        shares = int(shares)
    except ValueError:
        return False

    if shares < 1:
        return False
    else:
        return True

# Gets data from all a user's transaction
def getUserHistory(user_id):

    history = db.execute("SELECT * FROM transactions"
                        +" WHERE buyer_id = ? ORDER BY time DESC", user_id)


    return history

#export API_KEY=pk_0d6b21ce8cfe4adaa41243281a7b6b3d