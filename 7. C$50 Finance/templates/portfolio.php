<div>
    <ul class="nav nav-pills">
        <li><a href="quote.php">Quote</a></li>
        <li><a href="buy.php">Buy</a></li>
        <li><a href="sell.php">Sell</a></li>
        <li><a href="history.php">History</a></li>
        <li><a href="logout.php"><b>Log Out</b></a></li>
    </ul>
    </br>
    <ul class="nav nav-pills">
        <li><a href="deposit.php">Deposit Funds</a></li>
        <li><a href="withdraw.php">Withdraw Funds</a></li>
    </ul>
    </br>
    <?php if (!empty ($positions)): ?>
        <table class="table text-left">
                <tr>
                    <th>Symbol</th>
                    <th>Name</th>
                    <th>Shares</th>
                    <th>Price</th>
                    <th>TOTAL</th>
                </tr>
            <?php foreach ($positions as $position): ?>
                <tr>
                    <td><?= $position["symbol"] ?></td>
                    <td><?= $position["name"] ?></td>
                    <td><?= $position["shares"] ?></td>
                    <td><?= "$" . $position["price"] ?></td>
                    <td><?= "$" . $position["total_price"] ?></td>
                </tr>
            <?php endforeach ?>
                <tr>
                    <td colspan="4">CASH</td>
                    <td><?= "$" . $cash ?></td>
                </tr>
        </table>
    <?php else: ?>
        <h4>There are no shares currently.</h4>
        </br>
        <h5>CASH: <?= "$" . $cash ?></h5>
        </br>
    <?php endif ?>
</div>
