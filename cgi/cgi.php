<?php
    echo "rouali";
    // phpinfo();
    
    ?>
<style>
    body{
        background: #000;
        color: #fff;
    }
    h1
    {
        display: flex;
        justify-self: center;
    }
</style>

<?php
    // Check if a query parameter 'page' is set
    // $imagePath = '../tasir.jpeg';
    if (isset($_GET['page'])) {
        $page = $_GET['page'];
        
        // Use a switch statement to determine which content to display based on the 'page' parameter
        switch ($page) {
            case 'home':
                echo '<h1>Welcome to our Home Page</h1>';
                echo '<img src="' . '../badge.png' . '" alt="Image">';
                break;
            case 'about':
                echo '<h1>About Us</h1><p>We are a dynamic company...</p>';
                echo '<img style="width: 400px; height: 400px; margin-top: 20px; border-radius: 30px;" src="' . '../gggg.png' . '" alt="Image">';
                break;
            case 'contact':
                echo '<h1>Contact Us</h1><p>Reach out to us through our contact form...</p>';
                break;
            default:
                echo '<h1>Page not found</h1>';
        }
    } else {
        // If no 'page' parameter is set, display a default landing page
        echo '<h1>Welcome to our Default Landing Page</h1>';
    }
?>
<ul>
    <li><a href="?page=home">Home</a></li>
    <li><a href="?page=about">About Us</a></li>
    <li><a href="?page=contact">Contact Us</a></li>
</ul>