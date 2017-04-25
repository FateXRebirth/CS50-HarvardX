from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# check if it's integer
def RepresentsInt(s):
    try: 
        int(s)
        return True
    except ValueError:
        return False
# determine this action is buy or sell
def action(i):
    if i > 0:
        return 1
    elif i == 0:
        return 0
    else:
        return -1

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    """Show the portfolio"""
    # if no any user or trade
    result = db.execute("SELECT * FROM users WHERE id = :id", id = session["user_id"])
    # render index page with default data
    if not result:
        return render_template("index.html", total = usd(0))
    # get user's cash
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]["cash"]
    # check if success
    if not cash:
        raise RuntimeError
    # show all trade have been made
    trades = db.execute("SELECT symbol, name, share FROM trades WHERE username = :username", username = session["username"])
    # decorate price
    for n in range(len(trades)):
        trades[n]["price"] = usd(lookup(trades[n]["symbol"])["price"])
    # calculate and decorate cost
    for n in range(len(trades)):
        trades[n]["cost"] = usd(lookup(trades[n]["symbol"])["price"] * trades[n]["share"])
    # calculate and decorate total:
    total = 0
    for n in range(len(trades)):
        total += lookup(trades[n]["symbol"])["price"] * trades[n]["share"]
    total += cash
    # render index page with data
    return render_template("index.html", trades = trades, cash = usd(cash), total = usd(total))

@app.route("/login", methods = ["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["username"] = rows[0]["username"]

        # redirect user to home page
        flash("You were successfully logged in!")
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    flash("You were successfully logged out!")
    return redirect(url_for("login"))

@app.route("/register", methods = ["GET", "POST"])
def register():
    """Register user."""
    
    if request.method == "POST":
        # ensure username is not blank
        if not request.form.get("username"):
            return apology("Missing username!")
        # ensure password is not blank
        if not request.form.get("password"):
            return apology("Missing password!")
        # ensure second password is not blank
        if not request.form.get("confirmation"):
            return apology("Missing password!")
        # check if two passwords match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Password should be same!")
        # check if username is exists
        result = db.execute("SELECT * FROM users WHERE username = :username", username = request.form.get("username"))
        # if exist
        if result:
            return apology("Username exists already!")
        # encrypt password
        hash = pwd_context.encrypt(request.form.get("password"))
        # create account in database
        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :password)", username = request.form.get("username"), password = hash)
        # check if success
        if not result:
            raise RuntimeError
        # flash a message
        flash("You were successfully registered!")
        # redirect to login page
        return redirect(url_for("login"))
    else:
        # render register page
        return render_template("register.html")
        
@app.route("/quote", methods = ["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # ensure symbol is not blank
        if not request.form.get("symbol"):
            return apology("Missing symbol")
        # lookup this symbol
        result = lookup(request.form.get("symbol"))
        # ensure symbol is valid
        if result == None:
            return apology("Invalid symbol")
        # render quoted page with data
        return render_template("quoted.html", name=result["name"], price=result["price"], symbol=result["symbol"])
    else:
        # render quote page
        return render_template("quote.html")

@app.route("/buy", methods = ["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        # ensure symbol is not blank
        if not request.form.get("symbol"):
            return apology("Missing symbol")
        # ensure symbol is valid
        if lookup(request.form.get("symbol")) == None:
            return apology("Invalid symbol")
        # ensure share is not blank
        if not request.form.get("shares"):
            return apology("Missing share!")   
        # ensure share is a Integer
        if not RepresentsInt(request.form.get("shares")):
            return apology("Should be a positive integer")
        # ensure share is greater than 1 
        if int(request.form.get("shares")) <= 0:
            return apology("Should be a positive integer")
        
        # variable for saving data
        symbol = lookup(request.form.get("symbol"))["symbol"]
        name = lookup(request.form.get("symbol"))["name"]
        price = lookup(request.form.get("symbol"))["price"]
        share = int(request.form.get("shares"))
        cost = price * share
        
        # get user's cash
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]["cash"]
        # check if success
        if not cash:
            raise RuntimeError
        # check user have enough money
        if cost > cash:
            return apology("You don't have enough money!")
        # calculate the rest of money
        cash = cash - cost
        # update user's cash
        result = db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = cash, id = session["user_id"])
        # check if success
        if not result:
            raise RuntimeError
            
        # check if this company exists already
        result = db.execute("SELECT * FROM trades WHERE symbol = :symbol AND username = :username", symbol = symbol, username = session["username"])
        # not exist yet
        if not result:
            result = db.execute("INSERT INTO trades (username, symbol, name, share, price, cost) VALUES (:username, :symbol, :name, :share, :price, :cost)", symbol = symbol, name = name, share = share, price = price, cost = cost, username = session["username"])
        # exist already
        """
        else:
            new_share = share + result[0]['share']
            new_cost = cost + result[0]['cost']
            new_price = price
            result = db.execute("UPDATE trades SET share = :share, cost = :cost, price = :price WHERE symbol = :symbol AND username = :username", share = new_share, cost = new_cost, symbol = symbol, price = new_price, username = session["username"])
        """
        # check if success        
        if not result:
            raise RuntimeError
            
        # save this trade to history
        result = db.execute("INSERT INTO histories (username, symbol, share, price) VALUES (:username, :symbol, :share, :price)", symbol = symbol, share = share, price = price, username = session["username"])
        # check if success
        if not result:
            raise RuntimeError
        
        # flash a message    
        flash("You were successfully bought!")
        # redirect index page
        return redirect(url_for("index"))
        
    else:
        # render buy page
        return render_template("buy.html")

@app.route("/sell", methods = ["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        # ensure symbol is not blank
        if not request.form.get("symbol"):
            return apology("Missing symbol")
        # ensure symbol is valid
        if lookup(request.form.get("symbol")) == None:
            return apology("Invalid symbol")
        # ensure share is not blank
        if not request.form.get("shares"):
            return apology("Missing share!")   
        # ensure share is a Integer
        if not RepresentsInt(request.form.get("shares")):
            return apology("Should be a positive integer")
        # ensure share is greater than 1 
        if int(request.form.get("shares")) <= 0:
            return apology("Should be a positive integer")
        
        # variable for saving data
        symbol = lookup(request.form.get("symbol"))["symbol"]
        name = lookup(request.form.get("symbol"))["name"]
        price = lookup(request.form.get("symbol"))["price"]
        share = int(request.form.get("shares"))
        gain = price * share
       
        # check if this company exists already
        result = db.execute("SELECT * FROM trades WHERE symbol = :symbol AND username = :username", symbol = symbol, username = session["username"])
        # not exist yet
        if not result:
            return apology("You didn't buy this!")
        # exist already
        else:
            # calculate new data
            new_share = result[0]['share'] - share
            new_cost = result[0]['cost'] - gain
            new_price = price
            # if it have no any share
            if new_share == 0:
                # delete this from trade database
                result = db.execute("DELETE FROM trades WHERE symbol = :symbol AND username = :username", symbol = symbol, username = session["username"])
            # if it still own share
            elif new_share < 0:
                return apology("You didn't buy so much!")
            else:
                # update new data to trade database
                result = db.execute("UPDATE trades SET share = :share, cost = :cost, price = :price WHERE symbol = :symbol AND username = :username", share = new_share, cost = new_cost, symbol = symbol, price = new_price, username = session["username"])
        # check if success        
        if not result:
            raise RuntimeError
        
        # get user's cash
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]["cash"]
        # check if success
        if not cash:
            raise RuntimeError
        # calculate increasing money
        cash = cash + gain
        # update user's cash
        result = db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = cash, id = session["user_id"])
        # check if success
        if not result:
            raise RuntimeError
        
        # save this trade to history
        result = db.execute("INSERT INTO histories (username, symbol, share, price) VALUES (:username, :symbol, :share, :price)", symbol = symbol, share = (-share), price = price, username = session["username"])
        
        if not result:
            raise RuntimeError
            
        # flash a message    
        flash("You were successfully sold!")
        # redirect index page
        return redirect(url_for("index"))
        
    else:
        # render sell page
        return render_template("sell.html")
    
@app.route("/history", methods = ["GET", "POST"])
@login_required
def history():
    """Show history of transactions."""
    
    if request.method == "GET":
        # if no any history
        result = db.execute("SELECT * FROM histories WHERE username = :username", username = session["username"])
        # render history page
        if not result:
            return render_template("history.html")
        # get all history
        histories = db.execute("SELECT symbol, share, price, time FROM histories WHERE username = :username", username = session["username"])
        # check if success
        if not histories:
                raise RuntimeError
        # decorate price
        for n in range(len(histories)):
            histories[n]["price"] = usd(histories[n]["price"])
        # render history page with data
        return render_template("history.html", histories = histories, func = action)
        
    else:
        # empty whole history
        result = db.execute("DELETE FROM histories WHERE username = :username", username = session["username"])
        # check if success
        if not result:
            raise RuntimeError
        return render_template("history.html")
        
@app.route("/password", methods = ["GET", "POST"])
@login_required
def password():
    """Change password"""
    if request.method == "POST":
        # ensure old password is not blank
        if not request.form.get("oldPassword"):
            return apology("Missing old password!")
        # ensure password is not blank
        if not request.form.get("password"):
            return apology("Missing password!")
        # ensure second password is not blank
        if not request.form.get("confirmation"):
            return apology("Missing password!")
        # check if two passwords match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Password should be same!")
        # get use user's old password (encrypt)
        result = db.execute("SELECT hash FROM users WHERE id = :id", id = session["user_id"])
        # check if success
        if not result:
            raise RuntimeError
        # check if old password is correct or not
        if not pwd_context.verify(request.form.get("oldPassword"), result[0]["hash"]):
            return apology("Old password is not correct!")
        # encrypt new password
        newPassword = pwd_context.encrypt(request.form.get("password"))
        # commit new password to users
        result = db.execute("UPDATE users SET hash = :hash WHERE id = :id", id = session["user_id"], hash = newPassword)
        # check if success
        if not result:
            raise RuntimeError
        # flash a message
        flash("Please log in with new password!")
        # redirect to login page
        return redirect(url_for("login"))
    else:
        return render_template("password.html")

@app.route("/deposit", methods = ["GET", "POST"])
@login_required
def deposit():
    """Deposit the cash"""
    if request.method == "POST":
        # ensure symbol is not blank
        if not request.form.get("cash"):
            return apology("Missing cash")
        if not RepresentsInt(request.form.get("cash")):
            return apology("Should be a positive integer")
        # ensure share is greater than 1 
        if int(request.form.get("cash")) <= 0:
            return apology("Should be a positive integer")
        # additional cash
        addcash = float(request.form.get("cash"))
        # get user data
        result = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        # check if success
        if not result:
            raise RuntimeError
        # add new cash and owning cash
        cash = result[0]["cash"] + addcash
        # commit new cash to user
        result = db.execute("UPDATE users SET cash = :cash WHERE id = :id", id = session["user_id"], cash = cash)
        # check if success
        if not result:
            raise RuntimeError
        # commit to histories
        result = db.execute("INSERT INTO histories (username, share, price) VALUES (:username, :share, :price)", username = session["username"], share = 0, price = addcash)
        # check if success
        if not result:
            raise RuntimeError
        # flash a message
        flash("Your cash has increased!")
        # redirect to index page
        return redirect(url_for("index"))
    else:
        return render_template("deposit.html") 

