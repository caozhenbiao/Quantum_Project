
 
--删除共享
function C1()
	print("C1:删除共享")
 
end

--设置密码长度
function C2()
	print("C2:设置密码长度")
	local file = io.open("gp.inf", "a")
	io.output(file)
	io.write("[version]\n")
	io.write("signature=\"$CHICAGO$\"\n")
	io.write("[System Access]\n")
	io.write("MinimumPasswordLength = 6\n")
	io.write("PasswordComplexity = 1\n")
	io.close(file)
end


