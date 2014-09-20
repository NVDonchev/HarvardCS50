<?php

    // configuration
    require("../includes/config.php");
    
    // if the request is post, process
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // get the stock info from Yahoo Finance
        $stock = lookup($_POST["symbol"]);
        
        // if stock cannot be found - apologize
        if ($stock === false)
        {
            apologize("The symbol provided is invalid.");
        }
        
        // render the view
        render("quote_view.php", ["stock" => $stock, "title" => "Quote Price"]);
    }
    else
    {
        // if the request is not post, render the quote form
        render("quote_form.php", ["title" => "Quote Search"]);
    }
?>
