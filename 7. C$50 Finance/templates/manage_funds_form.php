<form action="<?= $action ?>.php" method="post">
    <fieldset>
        <div class="form-group">
            <input autofocus class="form-control" name="amount" placeholder="Amount" type="text"/>
        </div>
        <div class="form-group">
            <button type="submit" class="btn btn-default"><?= ucwords($action) ?> Funds</button>
        </div>
    </fieldset>
</form>
