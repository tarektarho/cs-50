import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get user's stocks and shares
    stocks = db.execute(
        "SELECT symbol, SUM(shares) as total_shares FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING total_shares > 0",
        user_id=session["user_id"],
    )

    # Get User's cash balance
    cash = db.execute(
        "SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"]
    )[0]["cash"]

    # Initialize variables for total values
    total_value = cash
    grand_total = cash

    # Iterate over stocks and add price and total value
    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock["name"] = quote["name"]
        stock["price"] = quote["price"]
        stock["value"] = stock["price"] * stock["total_shares"]
        total_value += stock["value"]
        grand_total += stock["value"]

    return render_template(
        "index.html",
        stocks=stocks,
        cash=cash,
        total_value=total_value,
        grand_total=grand_total,
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")
        if not symbol:
            return qpology("must provide symbol")
        elif not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("Must provide a positive integer number of shares")
        quote = lookup(symbol)
        if quote is None:
            return apology("symbol not found")
        price = quote["price"]
        total_cost = int(shares) * price
        cash = db.execute(
            "SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"]
        )[0]["cash"]

        if cash < total_cost:
            return apology("Not enough cash")
        # Update users table
        db.execute(
            "UPDATE users SET cash = cash - :total_cost WHERE id = :user_id",
            total_cost=total_cost,
            user_id=session["user_id"],
        )

        # Add the purchase to the history table
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (:user_id, :symbol, :shares, :price)",
            user_id=session["user_id"],
            symbol=symbol,
            shares=shares,
            price=price,
        )

        flash(f"Bought {shares} shares of {symbol} for {usd(total_cost)}!")
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Query database for user's transactions, ordered by most recent first
    transactions = db.execute(
        "SELECT * FROM transactions WHERE user_id = :user_id ORDER BY timestamp DESC",
        user_id=session["user_id"],
    )

    # Render history page with transactions
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/update-password", methods=["GET", "POST"])
def updatePasswrod():
    """Update password"""
    username = request.form.get("username")
    old_passwrod = request.form.get("old_passwrod")
    new_passwrod = request.form.get("new_passwrod")
    confirmation = request.form.get("confirmation")

    if request.method == "POST":
        # Query database for old password from user_id
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        if not username:
            flash("Username must be provided")
            return render_template("update-password.html")

        # Validate that passwrod is set
        elif not new_passwrod:
            flash("New Password must be provided")
            return render_template("update-password.html")

        # Ensure password and confirmation match
        elif new_passwrod != confirmation:
            flash("Must confirm password")
            return render_template("update-password.html")

        # Ensure passwrod does not already exsit
        if len(rows) != 1 or old_passwrod == new_passwrod:
            flash("invalid password")
            return render_template("update-password.html")

        # Update user passwrod
        try:
            db.execute(
                "UPDATE users SET hash = :new_passwrod WHERE username = :username",
                new_passwrod=generate_password_hash(new_passwrod),
                username=username,
            )
        except Exception as error:
            print(error)

        # Query database for newly inserted user
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Password has been changed!")
        return redirect("/")

    # User reached rout via GET (as by clicking a link or via redirect)
    else:
        return render_template("update-password.html")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if not quote:
            return apology("Invalid symbol", 400)
        return render_template("quote.html", quote=quote)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        if not username:
            return apology("Must provide username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("Must provide password", 400)

        # Ensure password confirmation was submitted
        elif not confirmation:
            return apology("Must confirm password", 400)

        # Ensure password and confirmation match
        elif password != confirmation:
            return apology("Must confirm password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Ensure username does not already exist
        if len(rows) != 0:
            return apology("Username alerady exists", 400)

        # Insert new user into database
        db.execute(
            "INSERT INTO users (username, hash) VALUES(?, ?)",
            username,
            generate_password_hash(password),
        )

        # Query database for newly inserted user
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached rout via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Get user's stocks
    stocks = db.execute(
        "SELECT symbol, SUM(shares) as total_shares FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING total_shares > 0",
        user_id=session["user_id"],
    )
    # If the user submits the form
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")
        if not symbol:
            return apology("Must provide symbol")
        elif not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("Must provide a positive integer number of shares")
        else:
            shares = int(shares)

        for stock in stocks:
            if stock["symbol"] == symbol:
                if stock["total_shares"] < shares:
                    return apology("Not enough shares")
                else:
                    # Get quote
                    quote = lookup(symbol)
                    if quote is None:
                        return apology("Symbol not found")
                    price = quote["price"]
                    total_sale = shares * price

                    # Update users table
                    db.execute(
                        "UPDATE users SET cash = cash + :total_sale WHERE id = :user_id",
                        total_sale=total_sale,
                        user_id=session["user_id"],
                    )

                    # Add the sale to the history table
                    db.execute(
                        "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (:user_id, :symbol, :shares, :price)",
                        user_id=session["user_id"],
                        symbol=symbol,
                        shares=-shares,
                        price=price,
                    )
                    flash(f"Sold {shares} shares of {symbol} for {usd(total_sale)}!")
                    # Upon completion, redirect the user to the home page.
                    return redirect("/")

        return apology("Symbol not found")

    else:
        return render_template("sell.html", stocks=stocks)
