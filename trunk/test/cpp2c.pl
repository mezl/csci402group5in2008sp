#!/usr/bin/perl
while(<>){
		if(m(^(.*)//(.*)$))
		{
				#print "$1 /* $2*/\n";#do convert
				print "$1 \n";#just delete comment 
		}else
		{
				print;
		}
}
