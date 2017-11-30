<!---
Elements taken from  W3layouts
License URL: http://creativecommons.org/licenses/by/3.0/
--->
<!DOCTYPE html>
<html lang="en">
<head>
<title>Powerhouse Surveillance</title>
<!-- for-mobile-apps -->
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script type="application/x-javascript"> addEventListener("load", function() { setTimeout(hideURLbar, 0); }, false);
		function hideURLbar(){ window.scrollTo(0,1); } </script>
<!-- //for-mobile-apps -->
<link href="css/bootstrap.css" rel="stylesheet" type="text/css" media="all" />
<link href="css/style.css" rel="stylesheet" type="text/css" media="all" />
<link href="css/font-awesome.css" rel="stylesheet"/> 

<!--web-fonts-->
<link href="//fonts.googleapis.com/css?family=Open+Sans:400,600,700,800" rel="stylesheet">
<link href="//fonts.googleapis.com/css?family=Lato:300,400,700" rel="stylesheet">
<!--//web-fonts-->
<!--//fonts-->
<!-- js -->
</head>
<body>
<!-- banner -->
	<div class="banner" id="home">
		<!-- header -->
		<header>
			<div class="container">

			<!-- navigation -->
			<nav class="navbar navbar-default">
				<!-- Brand and toggle get grouped for better mobile display -->
				<div class="navbar-header">
				  <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1" aria-expanded="false">
					<span class="sr-only">Toggle navigation</span>
					<span class="icon-bar"></span>
					<span class="icon-bar"></span>
					<span class="icon-bar"></span>
				  </button>				  
				<div class="w3-logo">
					<h1><a href="index.php">Powerhouse Surveillance</a></h1>
					
				</div>
				</div>

				<!-- Collect the nav links, forms, and other content for toggling -->
				<div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
				  <ul class="nav navbar-nav">
					<li><a class="active" href="index.php">Home</a></li>
					<li><a href="info.php">Info</a></li>
					<li><a href="webcam.html">Webcam</a></li>
					<li><a href="http://10.106.4.173" target="_blank">Wi-Fi Management System</a></li>
				  </ul>
				</div><!-- /.navbar-collapse -->
				 
			</nav>
			<div class="clearfix"></div>
		<!-- //navigation -->
			</div>
		</header>
	<!-- //header -->
	<!-- banner-text -->
	</div>
<!-- //banner -->


<!--sign-up-section-->
	<div class="signup" id="signup">
		<div class="container">
			<h5 class="title-w3">Welcome to your home surveillance system!</h5>
			<p>This system is set-up so you easily monitor your home while you are away through an online webcam. The webcam PIR sensor so pictures are only taken when movement is detected. Click any of the links above to get started! </p>
            <br>
            <img id="finalwebcam" src=".jpg" alt="Webcam in Casing">
            
			</div>
			
 </div>
<!--//sign-up-section-->
            
<!-- Footer -->
			<div class="copyright-wthree">
                <p>Sponsored by Northwestern University</p>
                <p>EECS 395/495: Engineering System Design</p>
                <p>Questions? Contact:
                    <a href="mailto:hannahemnett@gmail.com">hannahemnett@gmail.com
                    </a> 
                    or 
                    <a href="mailto:karan.shah@u.northwestern.edu">karan.shah@u.northwestern.edu
                    </a>
                </p>
			</div>



<!-- //Footer -->

	<a href="#home" class="scroll" id="toTop" style="display: block;"> <span id="toTopHover" style="opacity: 1;"> </span></a>
<!-- smooth scrolling -->

	
<script type="text/javascript" src="js/jquery-2.1.4.min.js"></script>
<script src="js/responsiveslides.min.js"></script>
							<script>
								// You can also use "$(window).load(function() {"
								$(function () {
								  // Slideshow 4
								  $("#slider3").responsiveSlides({
									auto: true,
									pager:true,
									nav:false,
									speed: 500,
									namespace: "callbacks",
									before: function () {
									  $('.events').append("<li>before event fired.</li>");
									},
									after: function () {
									  $('.events').append("<li>after event fired.</li>");
									}
								  });
							
								});
							 </script>
							 
							 
							 <script>
								// You can also use "$(window).load(function() {"
								$(function () {
								  // Slideshow 4
								  $("#slider1").responsiveSlides({
									auto: true,
									pager:false,
									nav:true,
									speed: 500,
									namespace: "callbacks",
									before: function () {
									  $('.events').append("<li>before event fired.</li>");
									},
									after: function () {
									  $('.events').append("<li>after event fired.</li>");
									}
								  });
							
								});
							 </script>
<!--gallery-->

<script type="text/javascript">
							$(window).load(function() {
								$("#flexiselDemo1").flexisel({
									visibleItems:3,
									animationSpeed: 1000,
									autoPlay: true,
									autoPlaySpeed: 3000,    		
									pauseOnHover: true,
									enableResponsiveBreakpoints: true,
									responsiveBreakpoints: { 
										portrait: { 
											changePoint:480,
											visibleItems: 1
										}, 
										landscape: { 
											changePoint:640,
											visibleItems:2
										},
										tablet: { 
											changePoint:768,
											visibleItems: 2
										}
									}
								});
								
							});
					</script>
					<script type="text/javascript" src="js/jquery.flexisel.js"></script>
<!--gallery-->



 <!-- start-smoth-scrolling -->
<script type="text/javascript" src="js/move-top.js"></script>
<script type="text/javascript" src="js/easing.js"></script>
<script type="text/javascript">
	jQuery(document).ready(function($) {
		$(".scroll").click(function(event){		
			event.preventDefault();
			$('html,body').animate({scrollTop:$(this.hash).offset().top},1000);
		});
	});
</script>
<!-- start-smoth-scrolling -->
<!-- here stars scrolling icon -->
	<script type="text/javascript">
		$(document).ready(function() {
			/*
				var defaults = {
				containerID: 'toTop', // fading element id
				containerHoverID: 'toTopHover', // fading element hover id
				scrollSpeed: 1200,
				easingType: 'linear' 
				};
			*/
								
			$().UItoTop({ easingType: 'easeOutQuart' });
								
			});
	</script>
<!-- //here ends scrolling icon -->
<!--js for bootstrap working-->
	<script src="js/bootstrap.js"></script>
<!-- //for bootstrap working -->


<!-- script-for-menu -->
					<script>					
						$("span.menu").click(function(){
							$(".top-nav ul").slideToggle("slow" , function(){
							});
						});
					</script>
					<!-- script-for-menu -->

</body>
</html>
