<?php

if (isset($_COOKIE["cookie"])) {
    unset($_COOKIE["cookie"]);
    setcookie("cookie", null, -1, '/');
}
header( "refresh:0; url=index.php" );

?>
