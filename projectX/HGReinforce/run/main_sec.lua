 
local REGEDIT = require "regedit"
 
--ɾ������
function B1()
	print("A1:ɾ������")
 	local retStr1 = REGEDIT.cmdPopen("sc config ������ start= disabled");
	
end

--�������볤��
function B2()
	print("A2:��ֹ�û������Զ���¼")
	--HKEY_LOCAL_MACHINE\Software\Microsoft\WindowsNT\CurrentVersion\Winlogon���½�һ������ΪAutoAdminLogon������ΪREG_DWORD��ֵΪ0�ļ�ֵ���������AutoAdminLogon ��ֱ�ӽ���ֵ��Ϊ0��
	 
end

--�ر�Windows�Զ����Ź���
function B3()
	print("B3:�ر�Windows�Զ����Ź���")
	
end

--���ÿ�Զ�̷��ʵ�ע���·������·��
function B4()
	print("B4:���ÿ�Զ�̷��ʵ�ע���·������·��")
	
end


--ɾ������ġ����ڵ��ʻ�
function B5()
	print("B6:ɾ������ġ����ڵ��ʻ�")
	
end

--Ĭ���˻���Ȩ�޿���
function B7()
	print("B4:Ĭ���˻���Ȩ�޿���")
	
end


--��������Զ������
function B8()
	print("B4:��������Զ������")
	
end


