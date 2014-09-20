<table class="table text-left">
    <tr>
        <th>Transaction</th>
        <th>Date/Time</th>
        <th>Symbol</th>
        <th>Shares</th>
        <th>Price</th>
    </tr>
    <?php foreach ($rows as $row): ?>
    <tr>
        <td><?= $row["transaction"] ?></td>
        <td><?= $row["time"] ?></td>
        <td><?= $row["symbol"] ?></td>
        <td><?= $row["shares"] ?></td>
        <td><?= "$" . $row["price"] ?></td>
    </tr>
    <?php endforeach ?>
</table>
