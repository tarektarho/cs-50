import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/<int:id>", methods=["DELETE"])
def delete(id):
    db.execute("DELETE FROM birthdays WHERE id = ?", id)

    return redirect("/")


@app.route("/", methods=["GET", "POST"])
def index():
    name = request.form.get("name")
    month = request.form.get("month")
    day = request.form.get("day")
    print(request.method)
    if request.method == "POST":
        # Insert data into database
        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?,?,?)", name, month, day)

        # Go back to homepage
        return redirect("/")

    else:
        # Query database and pass the to the template
        birthdays = db.execute("SELECT * FROM birthdays")

        # Display the entries in the database on index.html
        return render_template("index.html", birthdays=birthdays)