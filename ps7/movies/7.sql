SELECT title, rating FROM movies
JOIN ratings on id = ratings.movie_id
WHERE year = 2010 ORDER BY
ratings.rating DESC, title;