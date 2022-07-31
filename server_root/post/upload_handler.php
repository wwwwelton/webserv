<!DOCTYPE html>
<html>
    <head>
        <title>Upload</title>
    </head>
    <body>
        <pre>
        <?php
            // echo "raw: "; var_dump($HTTP_RAW_POST_DATA);
            
            echo "files: "; var_dump($_FILES);
            echo "post: "; var_dump($_POST);
            echo "server: "; var_dump($_SERVER);
            echo "request: "; var_dump($_REQUEST);

            // echo "\nRAW\n";
            // var_dump($HTTP_RAW_POST_DATA);
            // echo "\nFILES\n";
            // var_dump($_FILES);
            // echo "\nPOST\n";
            // var_dump($_POST);
            //
            // $f = fopen( 'php://stdin', 'r' );
            //
            // while( $line = fgets( $f ) ) {
            //   echo "line: " .$line;
            // }
            //
            // fclose( $f );

        ?>
        </pre>
    </body>
</html>
