from cs50 import get_string

str = get_string("Greeting: ")


if "Hello" in str:
    print("$0")

elif str[0] == "H" or str[0] == "h":
    print("$20")
else:
    print("$100")

