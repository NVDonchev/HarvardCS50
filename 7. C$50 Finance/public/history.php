<?php
    
    // configuration
    require("../includes/config.php");
    
    // get the history data from the database
    $rows = query("SELECT * FROM history");
    
    // render the view
    render("history_view.php", ["rows" => $rows, "title" => "Transaction History"]);
    
?>
