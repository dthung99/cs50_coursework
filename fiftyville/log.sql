-- Keep a log of any SQL queries you execute as you solve the mystery.
-- July 28, 2023
-- Humphrey Street

-- .tables

-- .schema crime_scene_reports

-- SELECT *
-- FROM crime_scene_reports
-- WHERE year = 2023 AND month = 7 AND day = 28 AND street = "Humphrey Street"
-- ;

-- | 295 | 2023 | 7     | 28  | Humphrey Street |
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |

-- 10:15am
-- Humphrey Street bakery
-- Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.

-- .schema bakery_security_logs

-- SELECT *
-- FROM bakery_security_logs
-- WHERE license_plate IN (
--     SELECT license_plate
--     FROM bakery_security_logs
--     WHERE year = 2023 AND month = 7 AND day = 28
--     AND (hour*60 + minute) <= (10*60+15)
-- )
-- AND year = 2023 AND month = 7 AND day = 28 AND (hour*60 + minute) >= (10*60+15)
-- ;

-- +-----+------+-------+-----+------+--------+----------+---------------+
-- | id  | year | month | day | hour | minute | activity | license_plate |
-- +-----+------+-------+-----+------+--------+----------+---------------+
-- | 260 | 2023 | 7     | 28  | 10   | 16     | exit     | 5P2BI95       |
-- | 261 | 2023 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
-- | 262 | 2023 | 7     | 28  | 10   | 18     | exit     | 6P58WS2       |
-- | 263 | 2023 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
-- | 264 | 2023 | 7     | 28  | 10   | 20     | exit     | G412CB7       |
-- | 265 | 2023 | 7     | 28  | 10   | 21     | exit     | L93JTIZ       |
-- | 266 | 2023 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
-- | 267 | 2023 | 7     | 28  | 10   | 23     | exit     | 0NTHK55       |
-- | 268 | 2023 | 7     | 28  | 10   | 35     | exit     | 1106N58       |
-- | 288 | 2023 | 7     | 28  | 17   | 15     | exit     | 13FNH73       |
-- | 290 | 2023 | 7     | 28  | 17   | 18     | exit     | R3G7486       |
-- +-----+------+-------+-----+------+--------+----------+---------------+


-- .schema interviews

-- SELECT *
-- FROM interviews
-- WHERE year = 2023 AND month = 7 AND day = 28
-- ;

-- | 161 | Ruth    | 2023 | 7     | 28  |
-- Sometime within ten minutes of the theft, I saw the thief get
-- into a car in the bakery parking lot and drive away. If you
-- have security footage from the bakery parking lot, you might
-- want to look for cars that left the parking lot in that time frame.                                                          |
-- | 162 | Eugene  | 2023 | 7     | 28  |
-- I don't know the thief's name, but it was someone I recognized. Earlier
-- this morning, before I arrived at Emma's bakery, I was walking by the ATM
-- on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
-- | 163 | Raymond | 2023 | 7     | 28  |
-- As the thief was leaving the bakery, they called someone who talked
-- to them for less than a minute. In the call, I heard the thief say that
-- they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket. |

-- .schema people

-- SELECT *
-- FROM people
-- WHERE license_plate IN
-- (
-- SELECT license_plate
-- FROM bakery_security_logs
-- WHERE license_plate IN (
--     SELECT license_plate
--     FROM bakery_security_logs
--     WHERE year = 2023 AND month = 7 AND day = 28
--     AND (hour*60 + minute) <= (10*60+15)
-- )
-- AND year = 2023 AND month = 7 AND day = 28 AND (hour*60 + minute) >= (10*60+15)
-- )
-- ;

-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 221103 | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
-- | 243696 | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
-- | 325548 | Brandon | (771) 555-6667 | 7874488539      | R3G7486       |
-- | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 745650 | Sophia  | (027) 555-1068 | 3642612721      | 13FNH73       |
-- +--------+---------+----------------+-----------------+---------------+


-- .schema atm_transactions
-- .schema bank_accounts
-- SELECT *
-- FROM people
-- WHERE id IN
-- (
--     SELECT person_id
--     FROM bank_accounts
--     WHERE account_number IN
--     (
--         SELECT account_number
--         FROM atm_transactions
--         WHERE year = 2023 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"
--     )
-- )
-- ;

-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 395717 | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | 438727 | Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
-- | 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
-- | 458378 | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+---------+----------------+-----------------+---------------+

-- .schema phone_calls

-- SELECT *
-- FROM phone_calls
-- WHERE year = 2023 AND month = 7 AND day = 28 AND duration <= 60
-- ;

-- SELECT *
-- FROM people
-- WHERE phone_number IN
-- (
--     SELECT caller
--     FROM phone_calls
--     WHERE year = 2023 AND month = 7 AND day = 28 AND duration <= 60
-- )
-- ;

-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 395717 | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 438727 | Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
-- | 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 561160 | Kathryn | (609) 555-5876 | 6121106406      | 4ZY7I8T       |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 907148 | Carina  | (031) 555-6622 | 9628244268      | Q12B3Z3       |
-- +--------+---------+----------------+-----------------+---------------+

-- .schema airports
-- .schema flights
-- .schema passengers

-- SELECT *
-- FROM people
-- WHERE license_plate IN
-- (
-- SELECT license_plate
-- FROM bakery_security_logs
-- WHERE license_plate IN (
--     SELECT license_plate
--     FROM bakery_security_logs
--     WHERE year = 2023 AND month = 7 AND day = 28
--     AND (hour*60 + minute) <= (10*60+15)
-- )
-- AND year = 2023 AND month = 7 AND day = 28 AND (hour*60 + minute) >= (10*60+15)
-- )
-- INTERSECT
-- SELECT *
-- FROM people
-- WHERE id IN
-- (
--     SELECT person_id
--     FROM bank_accounts
--     WHERE account_number IN
--     (
--         SELECT account_number
--         FROM atm_transactions
--         WHERE year = 2023 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"
--     )
-- )
-- INTERSECT
-- SELECT *
-- FROM people
-- WHERE phone_number IN
-- (
--     SELECT caller
--     FROM phone_calls
--     WHERE year = 2023 AND month = 7 AND day = 28 AND duration <= 60
-- )
-- ;

-- +--------+--------+----------------+-----------------+---------------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate |
-- +--------+--------+----------------+-----------------+---------------+
-- | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
-- | 514354 | Diana  | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+--------+----------------+-----------------+---------------+



-- .schema flights
-- .schema airports
-- SELECT *
-- FROM airports
-- ;
-- SELECT *
-- FROM flights
-- WHERE year = 2023 AND month = 7 AND day = 29 AND origin_airport_id IN (
--     SELECT id
--     FROM airports
--     WHERE city = "Fiftyville"
-- )
-- ORDER BY hour ASC, minute ASC
-- LIMIT 1
-- ;


-- .schema passengers


-- SELECT phone_calls.caller, cp.name, phone_calls.receiver, rp.name
-- FROM phone_calls
-- JOIN people AS cp
-- ON phone_calls.caller = cp.phone_number
-- JOIN people AS rp
-- ON phone_calls.receiver = rp.phone_number
-- WHERE phone_calls.caller IN
-- (
--     SELECT phone_number
--     FROM people
--     WHERE license_plate IN
--     (
--     SELECT license_plate
--     FROM bakery_security_logs
--     WHERE license_plate IN (
--         SELECT license_plate
--         FROM bakery_security_logs
--         WHERE year = 2023 AND month = 7 AND day = 28
--         AND (hour*60 + minute) <= (10*60+15)
--     )
--     AND year = 2023 AND month = 7 AND day = 28 AND (hour*60 + minute) >= (10*60+15)
--     )
--     INTERSECT
--     SELECT phone_number
--     FROM people
--     WHERE id IN
--     (
--         SELECT person_id
--         FROM bank_accounts
--         WHERE account_number IN
--         (
--             SELECT account_number
--             FROM atm_transactions
--             WHERE year = 2023 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"
--         )
--     )
--     INTERSECT
--     SELECT phone_number
--     FROM people
--     WHERE phone_number IN
--     (
--         SELECT caller
--         FROM phone_calls
--         WHERE year = 2023 AND month = 7 AND day = 28 AND duration <= 60
--     )
-- )
-- AND year = 2023 AND month = 7 AND day = 28 AND duration <= 60
-- ;
-- +----------------+--------+----------------+--------+
-- |     caller     |  name  |    receiver    |  name  |
-- +----------------+--------+----------------+--------+
-- | (367) 555-5533 | Bruce  | (375) 555-8161 | Robin  |
-- | (286) 555-6063 | Taylor | (676) 555-6554 | James  |
-- | (770) 555-1861 | Diana  | (725) 555-3243 | Philip |
-- +----------------+--------+----------------+--------+

-- SELECT *
-- FROM passengers
-- JOIN people
-- ON passengers.passport_number = people.passport_number
-- WHERE name IN ("Bruce", "Taylor", "Diana")
-- AND flight_id IN (
--     SELECT id
--     FROM flights
--     WHERE year = 2023 AND month = 7 AND day = 29 AND origin_airport_id IN (
--         SELECT id
--         FROM airports
--         WHERE city = "Fiftyville"
--     )
--     ORDER BY hour ASC, minute ASC
--     LIMIT 1
-- )
-- ;

-- +-----------+-----------------+------+--------+--------+----------------+-----------------+---------------+
-- | flight_id | passport_number | seat |   id   |  name  |  phone_number  | passport_number | license_plate |
-- +-----------+-----------------+------+--------+--------+----------------+-----------------+---------------+
-- | 36        | 5773159633      | 4A   | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 36        | 1988161715      | 6D   | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
-- +-----------+-----------------+------+--------+--------+----------------+-----------------+---------------+

-- 2 people left: Bruce and Taylor
