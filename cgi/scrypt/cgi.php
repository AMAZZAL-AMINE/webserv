<?php
    // echo "rouali";
    // phpinfo();   
?>

<style>
    body{
        background: #000;
        color: #fff;
        margin: 0;
        font-family: Arial, sans-serif;
        text-align: center;
    }
    ul {
      list-style-type: none;
      margin: 0;
      padding: 0;
      background-color: #333;
      overflow: hidden;
    }

    li {
      display: inline-block;
    }

    li a {
      display: block;
      color: white;
      padding: 14px 16px;
      text-decoration: none;
    }

    li a:hover {
      background-color: #111;
    }
    img {
      width: 400px;
      height: 400px;
      border-radius: 20px;
      margin-top: 20px;
      display: inline-block;
    }
</style>
<ul>
    <li><a href="?page=rouali">Rouali</a></li>
    <li><a href="?page=mamazzal">Mamazzal</a></li>
    <li><a href="?page=contact">Contact Us</a></li>
</ul>
<?php
    // Check if a query parameter 'page' is set
    // $imagePath = '../tasir.jpeg';
    if (isset($_GET['page'])) {
        $page = $_GET['page'];
        
        // Use a switch statement to determine which content to display based on the 'page' parameter
        switch ($page) {
            case 'rouali':
                echo '<h1>Welcome to Rouali</h1>';
                echo '<img style="object-fit: cover;" src="' . '../rouali.jpeg' . '" alt="Image">';
                break;
            case 'mamazzal':
                echo '<h1>Welcome to Mamazzal</h1>';
                echo '<img style="" src="' . '../gggg.png' . '" alt="Image">';
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