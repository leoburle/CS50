from flask import Flask, render_template

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html", home = "selected")

@app.route("/chuck")
def chuck():
    return render_template("Chuck_Rhoades.html",  title = "Chuck", chuck = "selected")

@app.route("/bobby")
def bobby():
    return render_template("Bobby_Axelrod.html", title = "Bobby", bobby = "selected")

@app.route("/taylor")
def taylor():
    return render_template("Taylor_Mason.html", title = "Taylor", taylor = "selected")

@app.route("/wendy")
def wendy():
    return render_template("Wendy_Rhoades.html", title = "Wendy", wendy = "selected")