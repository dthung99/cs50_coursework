SELECT songs.name
FROM songs
WHERE songs.artist_id IN
(
    SELECT artists.id
    FROM artists
    WHERE artists.name IN ("Post Malone")
)
;
