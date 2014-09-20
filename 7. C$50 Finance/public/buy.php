<?php
    
    // configuration
    require("../includes/config.php");
    
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // set transaction varable for the history info
        $transaction = "BUY";
        
        // check if shares is a positive number
        if (!preg_match("/^\d+$/", $_POST["shares"]))
        {
            apologize("Shares must be a positive number.");
        }
        
        // check if shares is a whole number
        if (floor($_POST["shares"]) != $_POST["shares"])
        {
            apologize("Shares must be a whole number.");
        }
        
        // capitalize stock symbol
        $symbol = strtoupper($_POST["symbol"]);
        
        // get current price of stock from Yahoo Finance
        $stock = lookup($_POST["symbol"]);
        
        // if stock cannot be found - apologize
        if ($stock === false)
        {
            apologize("The symbol provided is invalid.");
        }
                
        // get data for the stock from Yahoo Finance and calculate total price of stocks for buying
        $price = number_format($stock["price"] * $_POST["shares"], $decimals = 2, ".", "");
        
        // get the cash amount of the user
        $row = query("SELECT cash FROM users WHERE id = ?", $_SESSION["id"]);
        
        // check if the user have enough cash make the purchase
        if ($price > $row[0]["cash"])
        {
            apologize("You don't have enough cash to make this transaction.");
        }
        
        //insert transaction data into history database
        query("INSERT INTO history (transaction, symbol, shares, price) VALUES(?, ?, ?, ?)", $transaction, $symbol, $_POST["shares"], $price);
        
        // update the cash amount of the user
        query("UPDATE users SET cash = cash - ? WHERE id = ?", $price, $_SESSION["id"]);

        // insert or update the stocks that were bought
        query("INSERT INTO shares (user_id, symbol, shares) VALUES(?, ?, ?) ON DUPLICATE KEY UPDATE shares = shares + VALUES(shares)", $_SESSION["id"], $symbol, $_POST["shares"]);
          
        // redirect to index 
        redirect("index.php");
    }
    else
    {
        //  render the buy form
        render("buy_form.php", ["title" => "Buy Form"]);
    }

?>
