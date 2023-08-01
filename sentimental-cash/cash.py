from cs50 import get_float


# Propt the user for amount of change owed until non-negative value provided
while True:
    try:
        change_owed = get_float("Change owed: ")
        # Keep repeating the process if it's not an integer or it's a negative number
        if change_owed >= 0:
            break
    except ValueError:
        pass

# Convert the change owed to cents (multiply by 100 and round to the nearest int)
change_owed_cents = round(change_owed * 100)

# Initilize number of coins to 0
num_coins = 0


# Calculate number of coins to 0
while change_owed_cents > 0:
    if change_owed_cents >= 25:
        change_owed_cents -= 25
        num_coins += 1
    elif change_owed_cents >= 10:
        change_owed_cents -= 10
        num_coins += 1
    elif change_owed_cents >= 5:
        change_owed_cents -= 5
        num_coins += 1
    else:
        change_owed_cents -= 1
        num_coins += 1

# Print number of coins
print(num_coins)
