<?php

    // configuration
    require("../includes/config.php"); 
    
    // get the relevant data from the database
    $rows = query("SELECT user_id, symbol, shares FROM shares WHERE user_id = ?", $_SESSION["id"]);
    $cashes = query("SELECT cash FROM users WHERE id = ?", $_SESSION["id"]);
    $cash = number_format($cashes[0]["cash"], $decimals = 2);
    
    // assign the data to a positions array
    $positions = [];
    foreach ($rows as $row)
    {
        // get financial info from Yahoo Finance
        $stock = lookup($row["symbol"]);
        
        if ($stock !== false)
        {
            $total_price = number_format($stock["price"] * $row["shares"], $decimals = 2);
            $positions[] = [
                "name" => $stock["name"],
                "price" => number_format($stock["price"], $decimals = 2),
                "shares" => $row["shares"],
                "symbol" => $row["symbol"],
                "total_price" => $total_price
            ];
        }
    }
    
    // render portfolio by passing positions array
    render("portfolio.php", ["cash" => $cash, "positions" => $positions, "title" => "Portfolio"]);

?>
