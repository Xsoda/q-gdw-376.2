--[[
  - Q/GDW 376.2
    ParseDatagram return's lua table:
	{
		Len : ���ĳ���
		Dir : ���䷽��λ
		RouterId : ·�ɱ�ʶ
		CommModuleId : ͨѶģ���ʶ
		RelayLevel : �м�����
		ChannelId : �ŵ���ʶ
		Afn : Ӧ�ù�����
		Fn : ���ݵ�Ԫ��ʶ
		Data : Ӧ������
		DataLen : Ӧ�����ݳ���
		*** and it's retain ***
		SourceAddr : Դ��ַ
		RelayAddr �� �м̵�ַ
		DestinationAddr �� Ŀ�ĵ�ַ
	}
  - DL/T 645 2007
	ParsePacket return's lua table:
	{
	}
--]]

Datagram = {
	parse = function (t)
		local wrapper = require("luawrapper")
		return wrapper.ParseDatagram(t)
	end,
}

		