-- Keep a log of any SQL queries you execute as you solve the mystery.

-- airports              crime_scene_reports   people
-- atm_transactions      flights FK(origin_airport_id) & FK(destination_airport_id)              phone_calls
-- bakery_security_logs  interviews
-- bank_accounts FK(person_id)        passengers FK(flight_id)


--All you know is that the theft took place on July 28, 2021 and that it took place on Humphrey Street.

-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
-- Littering took place at 16:36. No known witnesses.

-- Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
-- | I don't know the thief's name, but it was someone I recognized. Earlier this morning,
-- before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
-- | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call,
-- I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket. |

-- -- Who the thief is,
-- -- What city the thief escaped to, and
-- -- Who the thief’s accomplice is who helped them escape

-- -- We know date July 28, 2021 let's find table that contain date information that have relation with place

-- List of tables with date that we can use: crime_scene_reports, interviews, atm_transactions, flights, phone_calls, bakery_security_logs

-- List of tables with streets names or locations: atm_transactions

SELECT DISTINCT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour >= 10 and minute >= 15;

SELECT transcript FROM interviews WHERE year = 2021 AND day = 28 AND month = 7;

SELECT DISTINCT caller FROM phone_calls WHERE day = 28 AND year = 2021 AND month = 7 AND duration < 60;


SELECT DISTINCT name, passport_number, license_plate FROM people WHERE phone_number in (SELECT DISTINCT caller FROM phone_calls WHERE day = 28 AND year = 2021 AND month = 7 AND duration < 60);


SELECT DISTINCT name, passport_number, phone_number FROM people WHERE license_plate in (5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7, L93JTIZ, 322W7JE, 0NTHK55, 1106N58, NRYN856, WD5M8I6, V47T75I, 94MV71O , FLFN3W0, 207W38T, 4468KVT, NAW9653, NAW9653, 94MV71O, 4468KVT, 207W38T,C194752,13FNH73, V47T75I, R3G7486, FLFN3W0, 4963D92);

SELECT DISTINCT name, passport_number, license_plate FROM people WHERE phone_number = '(996) 555-8899';

-- +----------------+----------------+
-- |     caller     |    receiver    |
-- +----------------+----------------+
-- | (130) 555-0289 | (996) 555-8899 |
-- | (499) 555-9472 | (892) 555-8872 |
-- | (367) 555-5533 | (375) 555-8161 |
-- | (499) 555-9472 | (717) 555-1342 |
-- | (286) 555-6063 | (676) 555-6554 |
-- | (770) 555-1861 | (725) 555-3243 |
-- | (031) 555-6622 | (910) 555-3251 |
-- | (826) 555-1652 | (066) 555-9701 |
-- | (338) 555-6650 | (704) 555-2131 |
-- +----------------+----------------+

-- +-------+-----------------+---------------+
-- | name  | passport_number | license_plate |
-- +-------+-----------------+---------------+
-- | Sofia | 1695452385      | G412CB7       |
-- +-------+-----------------+---------------+
SELECT name, phone_number, passport_number FROM people WHERE license_plate in ('1106N58', '322W7JE', '0NTHK55', '94KL13X');

+--------+
|  name  |
+--------+
| Taylor |
| Diana  |
| Kelsey |
| Bruce  |
+--------+


SELECT license_plate, activity, hour, minute FROM bakery_security_logs WHERE license_plate in (SELECT license_plate FROM people WHERE passport_number in (SELECT passport_number FROM passengers WHERE passport_number in (SELECT passport_number FROM people WHERE license_plate in ('1106N58', '322W7JE', '0NTHK55', '94KL13X'))));

SELECT full_name, id, city FROM airports WHERE city = 'Fiftyville';

SELECT passport_number FROM passengers WHERE passport_number in (SELECT passport_number FROM people WHERE license_plate in ('1106N58', '322W7JE', '0NTHK55', '94KL13X'));

SELECT name FROM people WHERE passport_number in (SELECT passport_number FROM passengers WHERE passport_number in (SELECT passport_number FROM people WHERE license_plate in ('1106N58', '322W7JE', '0NTHK55', '94KL13X')));

-- id = 8


SELECT DISTINCT account_number FROM atm_transactions WHERE year = 2021 AND day = 28 AND month = 7 AND atm_location LIKE 'Humphrey%';

SELECT person_id FROM bank_accounts WHERE account_number in (SELECT DISTINCT account_number FROM atm_transactions WHERE year = 2021 AND day = 28 AND month = 7 AND atm_location LIKE 'Humphrey%');

SELECT name, phone_number, license_plate FROM people WHERE id in (SELECT person_id FROM bank_accounts WHERE account_number in (SELECT DISTINCT account_number FROM atm_transactions WHERE year = 2021 AND day = 28 AND month = 7 AND atm_location LIKE 'Humphrey%')) AND license_plate NOT NULL;


SELECT license_plate FROM people WHERE id in (SELECT person_id FROM bank_accounts WHERE account_number in (SELECT DISTINCT account_number FROM atm_transactions WHERE year = 2021 AND day = 28 AND month = 7 AND atm_location LIKE 'Humphrey%')) AND license_plate NOT NULL;


SELECT license_plate, activity, hour, minute FROM bakery_security_logs WHERE year = 2021 AND day = 28 AND month = 7 AND hour < 11 AND license_plate in (SELECT license_plate FROM people WHERE id in (SELECT person_id FROM bank_accounts WHERE account_number in (SELECT DISTINCT account_number FROM atm_transactions WHERE year = 2021 AND day = 28 AND month = 7 AND atm_location LIKE 'Humphrey%')) AND license_plate NOT NULL);