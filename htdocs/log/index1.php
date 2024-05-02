<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $uploadDir = "/Users/rouali/Desktop/web_s/htdocs/uploads/";

    if (isset($_FILES["files"]) && $_FILES["files"]["error"] == 0) {
        $uploadFile = $uploadDir . basename($_FILES["files"]["name"]);
        if (file_exists($uploadFile)) {
            ?>
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Upload Success!</title>
                <style>
                    body {
                        font-family: Arial, sans-serif;
                        background-color: #f0f0f0;
                        text-align: center;
                    }
                    .container {
                        max-width: 600px;
                        margin: 100px auto;
                        padding: 20px;
                        background-color: #fff;
                        border-radius: 10px;
                        box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                    }
                    h1 {
                        color: #4CAF50;
                    }
                </style>
            </head>
            <body>
            <div class="container">
                <h1>Upload Successful!</h1>
                <p>The file <strong><?php echo htmlspecialchars(basename($_FILES["files"]["name"])) ?></strong> has been uploaded.</p>
                <p>Thank you for sharing!</p>
            </div>
            </body>
            </html>
            <?php
        } else {
            if (move_uploaded_file($_FILES["files"]["tmp_name"], $uploadFile))
            {
                ?>
                    <!DOCTYPE html>
                    <html lang="en">
                    <head>
                        <meta charset="UTF-8">
                        <meta name="viewport" content="width=device-width, initial-scale=1.0">
                        <title>Upload Success!</title>
                        <style>
                            body {
                                font-family: Arial, sans-serif;
                                background-color: #f0f0f0;
                                text-align: center;
                            }
                            .container {
                                max-width: 600px;
                                margin: 100px auto;
                                padding: 20px;
                                background-color: #fff;
                                border-radius: 10px;
                                box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                            }
                            h1 {
                                color: #4CAF50;
                            }
                        </style>
                    </head>
                    <body>
                    <div class="container">
                        <h1>Upload Successful!</h1>
                        <p>The file <strong><?php echo htmlspecialchars(basename($_FILES["files"]["name"])) ?></strong> has been uploaded.</p>
                        <p>Thank you for sharing!</p>
                    </div>
                    </body>
                    </html>
                    <?php
                echo "File is valid, and was successfully uploaded.";
            }
            else
            {
                ?>
                    <!DOCTYPE html>
                    <html lang="en">
                    <head>
                        <meta charset="UTF-8">
                        <meta name="viewport" content="width=device-width, initial-scale=1.0">
                        <title>Upload Failed!</title>
                        <style>
                            body
                            {
                                font-family: Arial, sans-serif;
                                background-color: #f0f0f0;
                                text-align: center;
                            }
                            .container
                            {
                                max-width: 600px;
                                margin: 100px auto;
                                padding: 20px;
                                background-color: #fff;
                                border-radius: 10px;
                                box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                            }
                            h1
                            {
                                color: red;
                            }
                        </style>
                    </head>
                    <body>
                    <div class="container">
                        <h1>Upload failed!</h1>
                        <p>Thank you!</p>
                    </div>
                    </body>
                    </html>
                <?php
            }
        }
    }
    else
    {
        ?>
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Upload Failed!!</title>
                <style>
                    body
                    {
                        font-family: Arial, sans-serif;
                        background-color: #f0f0f0;
                        text-align: center;
                    }
                    .container
                    {
                        max-width: 600px;
                        margin: 100px auto;
                        padding: 20px;
                        background-color: #fff;
                        border-radius: 10px;
                        box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                    }
                    h1
                    {
                        color: red;
                    }
                </style>
            </head>
            <body>
            <div class="container">
                <h1>Invalid file upload!</h1>
                <p>Thank you!</p>
            </div>
            </body>
            </html>
        <?php
    }
}
?>