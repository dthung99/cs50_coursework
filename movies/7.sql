SELECT movies.title, ratings.rating
FROM ratings
JOIN movies
ON movies.id = ratings.movie_id
WHERE movies.id IN
(   SELECT id
    FROM movies
    WHERE year IN (2010)
)
ORDER BY ratings.rating DESC, movies.title ASC
;

