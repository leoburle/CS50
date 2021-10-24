SELECT name from people JOIN
directors on people.id = person_id JOIN
ratings on directors.movie_id = ratings.movie_id
WHERE ratings.rating >= 9.0 GROUP BY people.id;