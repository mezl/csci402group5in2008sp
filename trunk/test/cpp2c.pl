#!/usr/bin/perl
while(<>){
		if(m(^(.*)//(.*)$))
		{
				print "$1 /* $2*/\n";
		}else
		{
				print;
		}
}
