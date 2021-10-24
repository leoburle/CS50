SELECT title FROM movies JOIN
stars on movies.id = stars.movie_id
JOIN people on stars.person_id = people.id
JOIN ratings on ratings.movie_id = movies.id
WHERE people.name = "Chadwick Boseman"
ORDER BY ratings.rating DESC LIMIT 5;