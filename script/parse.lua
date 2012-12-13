--[[
  - Q/GDW 376.2
    ParseDatagram return's lua table:
	{
		Len : ���ĳ���
		Dir : ���䷽��λ
		RouterId : ·�ɱ�ʶ
		CommModuleId : ͨѶģ���ʶ
		CommMode : ͨѶ��ʽ
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
		*** ���б������� ***
		MeterChannelFeature : ���ͨ������
		PhaseLineMark: ʵ�����߱�ʶ
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
	show = function (r)
		local CM_STR = {
			[1] = "������ʽ·���ز�ͨѶ��",
			[2] = "���ֲ�ʽ·���ز�ͨѶ��",
			[10] = "��΢��������ͨѶ��",
			[20] = "����̫��ͨѶ��",
			[11] = "�����á�"
		}
		local CMI_STR = {
			[0] = "���Լ�������ͨ��ģ�������",
			[1] = "�����ز����ͨ��ģ�������"
		}
		local RI_STR = {
			[0] = "��ͨ��ģ���·�ɡ�",
			[1] = "��ͨѶģ�鲻��·�ɡ�"
		}
		ret = ""
		if type(r) == "table" then
			if r.error == nil then				
				ret = ret.."���ĳ���: "..r.Len
				ret = ret.."ͨѶ��ʽ: "..r.CommMode.." "..CM_STR[r.CommMode] or CM_STR[11]
				ret = ret.."�м̼���: "..r.RelayLevel.." "..("�����м̡�" or (string.format("��%d���м̡�", r.RelayLevel) and r.RelayLevel ~= 0))
				ret = ret.."ͨѶģ���ʶ: "..r.CommModuleId.." "..CMI_STR[r.CommModuleId]
				ret = ret.."·�ɱ�ʶ: "..r.RouterId.." "..RI_STR[r.RouterId]
				ret = ret.."�ŵ���ʶ: "..r.ChannelId.." "..("�������ŵ���" or (string.format("����%d�ŵ���", r.ChannelId) and r.ChannelId ~= 0))
				if r.Dir == 1 then
					local MCF_STR = {
						[0] = "��������",
						[1] = "���ز������ŵ�Ϊ���๩�� �߼��ŵ�Ϊ���ŵ���",  
						[2] = "���ز������ŵ�Ϊ���๩�� �߼��ŵ�Ϊ���ŵ���",  
						[3] = "���ز������ŵ�Ϊ���๩�� �߼��ŵ�Ϊ���ŵ���",  
						[4] = "���ز������ŵ�Ϊ���๩�� �߼��ŵ�Ϊ���ŵ���",  
						[5] = "��δ֪��ͨ��������"
					}
					ret = ret.."���ͨ������: "..r.MeterChannelFeature.." "..MCF_STR[5] or MCF_STR[r.MeterChannelFeature]
					ret = ret.."ʵ�����߱�ʶ: "..r.PhaseLineMark.." "..("����ȷ����" or (string.format("����%d�ࡿ", r.PhaseLineMark) and r.PhaseLineMark ~= 0))
				end
				local AFN_STR= {
					[0x0] = "ȷ��/����",
					[0x1] = "��ʼ��",
					[0x2] = "����ת��",
					[0x3] = "��ѯ����",
					[0x4] = "��·�ӿڼ��",
					[0x5] = "��������",
					[0x6] = "�����ϱ�",
					[0x10] = "·�ɲ�ѯ",
					[0x11] = "·������",
					[0x12] = "·�ɿ���",
					[0x13] = "·������ת��",
					[0x14] = "·�����ݳ���",
					[0xF0] = "�ڲ�����",
				}
				local FN_STR = {
					[0x0] = {
						[0x1] = "ȷ��",
						[0x2] = "����"
					},
					[0x1] = {
						[0x1] = "Ӳ����ʼ��",
						[0x2] = "��������ʼ��",
						[0x3] = "��������ʼ��"
					},
					[0x2] = {
						[0x1] = "ת������"
					},
					[0x3] = {
						[0x1] = "���̴���Ͱ汾��Ϣ",
						[0x2] = "����ֵ",
						[0x3] = "�ز��ӽڵ�������Ϣ",
						[0x4] = "�ز����ڵ��ַ",
						[0x5] = "�ز����ڵ�״̬�ֺ��ز�����",
						[0x6] = "�ز����ڵ����״̬",
						[0x7] = "��ѯ̨�����"
					},
					[0x4] = {
						[0x1] = "���Ͳ���",
						[0x2] = "�ز��ӽڵ����"
					},
					[0x5] = {
						[0x1] = "�����ز����ڵ��ַ",
						[0x2] = "�����ز��ӽڵ��ϱ�",
						[0x3] = "�����㲥",
						[0x4] = "����̨�����"
					},
					[0x6] = {
						[0x1] = "�ϱ��ز��ӽڵ���Ϣ",
						[0x2] = "�ϱ���������"
					},
					[0x10] = {
						[0x1] = "�ز��ӽڵ�����",
						[0x2] = "�ز��ӽڵ���Ϣ",
						[0x3] = "ָ���ز��ӽڵ����һ���м�·����Ϣ",
						[0x4] = "·������״̬",
						[0x5] = "Ϊ�����ɹ����ز��ӽڵ���Ϣ",
						[0x6] = "����ע����ز��ӽڵ���Ϣ"
					},
					[0x11] = {
						[0x1] = "����ز��ӽڵ�",
						[0x2] = "ɾ���ز��ӽڵ�",
						[0x3] = "�����ز��ӽڵ�Ŵ��м�·��",
						[0x4] = "���ù���ģʽ",
						[0x5] = "�����ز��ӽڵ�����ע��"
					},
					[0x12] = {
						[0x1] = "����",
						[0x2] = "��ͣ",
						[0x3] = "�ָ�"
					},
					[0x13] = {
						[0x1] = "����ز��ӽڵ�"
					},
					[0x14] = {
						[0x1] = "·�����󳭶�����"
					}
				}
				ret = ret..string.format("Ӧ�ù�����: %x", r.Afn).." "..string.format("��%s��", AFN_STR[r.Afn] or "δ֪AFN")
				ret = ret.."��Ϣ���ʶ: "..r.Fn.." "..string.format("��%s��", FN_STR[r.Afn][r.Fn]) or "��δ֪��ʶ��"
				ret = ret.."���ݵ�Ԫ����: "..r.DataLen
				if r.Data ~= nil then
					ret = ret.."���ݵ�Ԫ����:"
					for _, v in pairs(r.Data) do
						ret = ret..string.format(" %x", v)
					end
				end
			else
				for _, v in pairs(r.error) do
					ret = ret.." "..v
				end
			end
		end
		return ret
	end,
}

		