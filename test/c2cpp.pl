#!/usr/bin/perl
$flag = 0;
while(<>){
		#handle single line /*comment*/
		if(m(^(.*)/\*(.*)\*/\s*))
		{
			print "$1//$2\n";
		}
		#handle mutiple line commont
		elsif(m(^(.*)/\*(.*)))
		{
			$flag = 1;#start to add //
				print "$1//$2\n";
		}
		elsif(m(^(.*)\*/(.*)))
		{
			
			$flag = 0;#end of add//
			print "//$1\n$2";
		}		

		else
		{
				if($flag){	print "//$_";}
				else{	print;}
		}
}
