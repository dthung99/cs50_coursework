SELECT AVG(songs.energy)
FROM songs
WHERE songs.artist_id IN
(
    SELECT artists.id
    FROM artists
    WHERE artists.name IN ("Drake")
)
;
