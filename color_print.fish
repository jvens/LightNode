function color_print
	for i in $argv
		set_color $i
		echo -n X
	end
	echo ""
end
