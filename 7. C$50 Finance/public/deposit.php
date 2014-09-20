<?php

    // configuration
    require("../includes/config.php");
    
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // check if amount is a positive number
        if (!preg_match("/^\d+$/", $_POST["amount"]))
        {
            apologize("Shares must be a positive number.");
        }
        
        // update the cash amount of the user
        query("UPDATE users SET cash = cash + ? WHERE id = ?", number_format($_POST["amount"], $decimals = 2, ".", ""), $_SESSION["id"]);
        
        // redirect to index 
        redirect("index.php");
    }
    else
    {
        // render the view, passing it the manage action
        render("manage_funds_form.php", ["action" => "deposit", "title" => "Manage Funds"]);
    }
    
?>
