<style>
  * {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
  }
  body{
    background-color: #000;
    color: #fff;
    text-align: center;
    font-family: Arial, Helvetica, sans-serif;
  }
  .gg {
    height: 100vh;
    display: flex;
    align-items: center;
    justify-content: center;
    flex-direction: column;
  }
  .gg-img {
    width: 400px;
    height: 400px;
    margin-top: 20px;
    border-radius: 30px;
  }
  input {
    padding: 10px;
    margin: 10px;
    border-radius: 5px;
    border: 1px solid #fff;
    background-color: transparent;
    color: #fff;
    outline: none;
    width: 100%;
  }
  button {
    padding: 10px;
    margin: 10px;
    width: 100%;
    border-radius: 5px;
    border: 1px solid #fff;
    background-color: transparent;
    color: #fff;
    outline: none;
    cursor: pointer;
  }
  .header {
    display: flex;
    align-items: center;
    justify-content: space-evenly;
  }
  .form {
    display: flex;
    align-items: center;
    justify-content: center;
    flex-direction: column;
    margin-left: -100px;
    background-color: #00000073;
    padding: 50px;
    border-radius: 10px;
    backdrop-filter: blur(10px);
    shape-rendering:  crispEdges;
    box-shadow: 0 0 10px 0 rgb(161, 145, 145);
  }
  .aa {
    text-decoration: none;
    color: rgb(255, 255, 255);
    padding: 10px;
    margin: 10px;
    width: 100%;
    border-radius: 5px;
    border: 1px solid #fff;
    background-color: transparent;
    color: #fff;
    outline: none;
    cursor: pointer;
  }
  .line {
    width: 50px;
    height: 1px;
    margin-top: 20px;
    margin-bottom: 20px;
    background-color: rgba(255, 255, 255, 0.307);
  }
</style>

<?php

echo "<div class='gg'>
    <div class='header'>
      <img
        class='gg-img'
        src='/gggg.png' loading='lazy'
      />

      <form class='form' action='/' method='post'>
        <p>Send data</p>
        <input type='text' name='name' placeholder='name' />
        <input type='text' name='email' placeholder='email' />
        <button >
          submit
        </button>
        <div class='line'></div>
        <a
          class='aa'
          href='/upload.html' >
          Or upload file
        </a>
      </form>
    </div>
  </div>"

?>