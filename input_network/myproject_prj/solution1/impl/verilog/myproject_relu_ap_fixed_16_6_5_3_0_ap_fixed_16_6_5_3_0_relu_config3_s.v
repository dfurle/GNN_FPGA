// ==============================================================
// Generated by Vitis HLS v2023.1
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
// ==============================================================

`timescale 1 ns / 1 ps 

module myproject_relu_ap_fixed_16_6_5_3_0_ap_fixed_16_6_5_3_0_relu_config3_s (
        ap_ready,
        data_4_val,
        data_5_val,
        data_7_val,
        ap_return_0,
        ap_return_1,
        ap_return_2
);


output   ap_ready;
input  [15:0] data_4_val;
input  [15:0] data_5_val;
input  [15:0] data_7_val;
output  [15:0] ap_return_0;
output  [15:0] ap_return_1;
output  [15:0] ap_return_2;

wire   [0:0] icmp_ln45_fu_52_p2;
wire   [14:0] trunc_ln43_18_fu_48_p1;
wire   [14:0] datareg_fu_58_p3;
wire   [0:0] icmp_ln45_17_fu_70_p2;
wire   [14:0] trunc_ln43_17_fu_44_p1;
wire   [14:0] datareg_17_fu_76_p3;
wire   [0:0] icmp_ln45_18_fu_88_p2;
wire   [14:0] trunc_ln43_fu_40_p1;
wire   [14:0] datareg_18_fu_94_p3;
wire   [15:0] zext_ln45_fu_66_p1;
wire   [15:0] zext_ln45_17_fu_84_p1;
wire   [15:0] zext_ln45_18_fu_102_p1;

assign ap_ready = 1'b1;

assign datareg_17_fu_76_p3 = ((icmp_ln45_17_fu_70_p2[0:0] == 1'b1) ? trunc_ln43_17_fu_44_p1 : 15'd0);

assign datareg_18_fu_94_p3 = ((icmp_ln45_18_fu_88_p2[0:0] == 1'b1) ? trunc_ln43_fu_40_p1 : 15'd0);

assign datareg_fu_58_p3 = ((icmp_ln45_fu_52_p2[0:0] == 1'b1) ? trunc_ln43_18_fu_48_p1 : 15'd0);

assign trunc_ln43_17_fu_44_p1 = data_5_val[14:0];

assign trunc_ln43_18_fu_48_p1 = data_4_val[14:0];

assign trunc_ln43_fu_40_p1 = data_7_val[14:0];

assign zext_ln45_17_fu_84_p1 = datareg_17_fu_76_p3;

assign zext_ln45_18_fu_102_p1 = datareg_18_fu_94_p3;

assign zext_ln45_fu_66_p1 = datareg_fu_58_p3;

assign ap_return_0 = zext_ln45_fu_66_p1;

assign ap_return_1 = zext_ln45_17_fu_84_p1;

assign ap_return_2 = zext_ln45_18_fu_102_p1;

assign icmp_ln45_17_fu_70_p2 = (($signed(data_5_val) > $signed(16'd0)) ? 1'b1 : 1'b0);

assign icmp_ln45_18_fu_88_p2 = (($signed(data_7_val) > $signed(16'd0)) ? 1'b1 : 1'b0);

assign icmp_ln45_fu_52_p2 = (($signed(data_4_val) > $signed(16'd0)) ? 1'b1 : 1'b0);

endmodule //myproject_relu_ap_fixed_16_6_5_3_0_ap_fixed_16_6_5_3_0_relu_config3_s