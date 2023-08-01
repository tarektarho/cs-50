-- write a SQL query that lists the names of songs that are by Post Malone.
-- Your query should output a table with a single column for the name of each song.

SELECT name FROM songs where artist_id = (SELECT id FROM artists WHERE name = 'Post Malone');