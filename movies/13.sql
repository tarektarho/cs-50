-- write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.
-- Your query should output a table with a single column for the name of each person.
-- There may be multiple people named Kevin Bacon in the database. Be sure to only select the Kevin Bacon born in 1958.
-- Kevin Bacon himself should not be included in the resulting list.

SELECT DISTINCT people.name
FROM people
JOIN stars AS stars1 ON people.id = stars1.person_id
JOIN movies AS movies1 ON stars1.movie_id = movies1.id
JOIN stars AS stars2 ON movies1.id = stars2.movie_id
JOIN people AS kevin_bacon ON stars2.person_id = kevin_bacon.id
WHERE kevin_bacon.name = 'Kevin Bacon' AND kevin_bacon.birth = 1958 AND people.name != 'Kevin Bacon';
