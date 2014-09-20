<?php
    
    // configuration
    require("../includes/config.php");
    
    // if the request method is post, process
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // check if the user selected a symbol
        if (empty($_POST["symbol"]))
        {
            apologize("You must select a stock symbol.");
        }
        
        // set transaction varable for the history info
        $transaction = "SELL";
        
        // get the shares number
        $row = query("SELECT shares FROM shares WHERE user_id = ? AND symbol = ?", $_SESSION["id"], $_POST["symbol"]);
        
        // get current price of stock from Yahoo Finance
        $stock = lookup($_POST["symbol"]);
        
        // get the total price of shares
        $price = number_format($stock["price"] * $row[0]["shares"], $decimals = 2, ".", "");
        
        //insert transaction data into history database
        query("INSERT INTO history (transaction, symbol, shares, price) VALUES(?, ?, ?, ?)", $transaction, $_POST["symbol"], $row[0]["shares"], $price);
        
        // update the cash amount of the user
        query("UPDATE users SET cash = cash + ? WHERE id = ?", $price, $_SESSION["id"]);
        
        // delete the stocks that was selled
        query("DELETE FROM shares WHERE user_id = ? AND symbol = ?", $_SESSION["id"], $_POST["symbol"]);
        
        // redirect to index 
        redirect("index.php");
    }
    else
    {
        // if the request method is not post, render the sell form
        $symbols = query("SELECT symbol FROM shares WHERE user_id = ?", $_SESSION["id"]);
        render("sell_form.php", ["symbols" => $symbols, "title" => "Sell Form"]);
    }
    
?>
