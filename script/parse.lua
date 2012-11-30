--[[
  - Q/GDW 376.2
    ParseDatagram return's lua table:
	{
		Len : 报文长度
		Dir : 传输方向位
		RouterId : 路由标识
		CommModuleId : 通讯模块标识
		CommMode : 通讯方式
		RelayLevel : 中级级别
		ChannelId : 信道标识
		Afn : 应用功能码
		Fn : 数据单元标识
		Data : 应用数据
		DataLen : 应用数据长度
		*** and it's retain ***
		SourceAddr : 源地址
		RelayAddr ： 中继地址
		DestinationAddr ： 目的地址
		*** 上行报文增加 ***
		MeterChannelFeature : 电表通道特征
		PhaseLineMark: 实测相线标识
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
			[1] = "【集中式路由载波通讯】",
			[2] = "【分布式路由载波通讯】",
			[10] = "【微功率无线通讯】",
			[20] = "【以太网通讯】",
			[11] = "【备用】"
		}
		local CMI_STR = {
			[0] = "【对集中器的通信模块操作】",
			[1] = "【对载波表的通信模块操作】"
		}
		local RI_STR = {
			[0] = "【通信模块带路由】",
			[1] = "【通讯模块不带路由】"
		}
		ret = ""
		if type(r) == "table" then
			if r.error == nil then				
				ret = ret.."报文长度: "..r.Len
				ret = ret.."通讯方式: "..r.CommMode.." "..CM_STR[r.CommMode] or CM_STR[11]
				ret = ret.."中继级别: "..r.RelayLevel.." "..("【无中继】" or (string.format("【%d级中继】", r.RelayLevel) and r.RelayLevel ~= 0))
				ret = ret.."通讯模块标识: "..r.CommModuleId.." "..CMI_STR[r.CommModuleId]
				ret = ret.."路由标识: "..r.RouterId.." "..RI_STR[r.RouterId]
				ret = ret.."信道标识: "..r.ChannelId.." "..("【不分信道】" or (string.format("【第%d信道】", r.ChannelId) and r.ChannelId ~= 0))
				if r.Dir == 1 then
					local MCF_STR = {
						[0] = "【保留】",
						[1] = "【载波物理信道为单相供电 逻辑信道为单信道】",  
						[2] = "【载波物理信道为单相供电 逻辑信道为两信道】",  
						[3] = "【载波物理信道为单相供电 逻辑信道为三信道】",  
						[4] = "【载波物理信道为三相供电 逻辑信道为三信道】",  
						[5] = "【未知的通道特征】"
					}
					ret = ret.."电表通道特征: "..r.MeterChannelFeature.." "..MCF_STR[5] or MCF_STR[r.MeterChannelFeature]
					ret = ret.."实测相线标识: "..r.PhaseLineMark.." "..("【不确定】" or (string.format("【第%d相】", r.PhaseLineMark) and r.PhaseLineMark ~= 0))
				end
				local AFN_STR= {
					[0x0] = "确认/否认",
					[0x1] = "初始化",
					[0x2] = "数据转发",
					[0x3] = "查询数据",
					[0x4] = "链路接口检测",
					[0x5] = "控制命令",
					[0x6] = "主动上报",
					[0x10] = "路由查询",
					[0x11] = "路由设置",
					[0x12] = "路由控制",
					[0x13] = "路由数据转发",
					[0x14] = "路由数据抄读",
					[0xF0] = "内部调试",
				}
				local FN_STR = {
					[0x0] = {
						[0x1] = "确认",
						[0x2] = "否认"
					},
					[0x1] = {
						[0x1] = "硬件初始化",
						[0x2] = "参数区初始化",
						[0x3] = "数据区初始化"
					},
					[0x2] = {
						[0x1] = "转发命令"
					},
					[0x3] = {
						[0x1] = "厂商代码和版本信息",
						[0x2] = "噪音值",
						[0x3] = "载波从节点侦听信息",
						[0x4] = "载波主节点地址",
						[0x5] = "载波主节点状态字和载波速率",
						[0x6] = "载波主节点干扰状态",
						[0x7] = "查询台区编号"
					},
					[0x4] = {
						[0x1] = "发送测试",
						[0x2] = "载波从节点点名"
					},
					[0x5] = {
						[0x1] = "设置载波主节点地址",
						[0x2] = "允许载波从节点上报",
						[0x3] = "启动广播",
						[0x4] = "设置台区编号"
					},
					[0x6] = {
						[0x1] = "上报载波从节点信息",
						[0x2] = "上报抄读数据"
					},
					[0x10] = {
						[0x1] = "载波从节点数量",
						[0x2] = "载波从节点信息",
						[0x3] = "指定载波从节点的上一级中继路由信息",
						[0x4] = "路由运行状态",
						[0x5] = "为抄读成功的载波从节点信息",
						[0x6] = "主动注册的载波从节点信息"
					},
					[0x11] = {
						[0x1] = "添加载波从节点",
						[0x2] = "删除载波从节点",
						[0x3] = "设置载波从节点古代中继路径",
						[0x4] = "设置工作模式",
						[0x5] = "激活载波从节点主动注册"
					},
					[0x12] = {
						[0x1] = "重启",
						[0x2] = "暂停",
						[0x3] = "恢复"
					},
					[0x13] = {
						[0x1] = "监控载波从节点"
					},
					[0x14] = {
						[0x1] = "路由请求抄读内容"
					}
				}
				ret = ret..string.format("应用功能码: %x", r.Afn).." "..string.format("【%s】", AFN_STR[r.Afn] or "未知AFN")
				ret = ret.."信息类标识: "..r.Fn.." "..string.format("【%s】", FN_STR[r.Afn][r.Fn]) or "【未知标识】"
				ret = ret.."数据单元长度: "..r.DataLen
				if r.Data ~= nil then
					ret = ret.."数据单元内容:"
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

		