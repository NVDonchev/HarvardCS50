<?php

    // configuration
    require("../includes/config.php");

    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate input
        if (!isset($_POST["username"]))
        {
            alopogize("You must provide a username.");
        }
        if (!isset($_POST["password"]))
        {
            alopogize("You must provide a password.");
        }
        if ($_POST["password"] !== $_POST["confirmation"])
        {
            alopogize("Your password comfirmation do not match the password provided.");
        }
        
        // insert the new user into the database
        $success = query("INSERT INTO users (username, hash, cash) VALUES(?, ?, 10000.00)", $_POST["username"], crypt($_POST["password"]));

        // if insertion is not successful - apologize
        if ($success === false)
        {
            apologize("There was a problem with your registration. Possibly the username you provided is already used.");
        }
        
        // get the newly added user's id from the database and add the id to _SESSION
        $rows = query("SELECT LAST_INSERT_ID() AS id");
        $id = $rows[0]["id"];
        $_SESSION["id"] = $id;
        
        // redirect to the index page
        redirect("index.php");
    }
    else
    {
        // else render form
        render("register_form.php", ["title" => "Register"]);
    }

?>
