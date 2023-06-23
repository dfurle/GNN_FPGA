// ==============================================================
// Generated by Vitis HLS v2023.1
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
// ==============================================================

`timescale 1 ns / 1 ps 

module myproject_relu_ap_fixed_16_6_5_3_0_ap_fixed_16_6_5_3_0_relu_config7_s (
        ap_ready,
        data_0_val,
        data_1_val,
        data_2_val,
        data_3_val,
        data_4_val,
        data_5_val,
        ap_return_0,
        ap_return_1,
        ap_return_2,
        ap_return_3,
        ap_return_4,
        ap_return_5
);


output   ap_ready;
input  [15:0] data_0_val;
input  [15:0] data_1_val;
input  [15:0] data_2_val;
input  [15:0] data_3_val;
input  [15:0] data_4_val;
input  [15:0] data_5_val;
output  [15:0] ap_return_0;
output  [15:0] ap_return_1;
output  [15:0] ap_return_2;
output  [15:0] ap_return_3;
output  [15:0] ap_return_4;
output  [15:0] ap_return_5;

wire   [0:0] icmp_ln45_fu_88_p2;
wire   [14:0] trunc_ln43_11_fu_84_p1;
wire   [14:0] datareg_fu_94_p3;
wire   [0:0] icmp_ln45_7_fu_106_p2;
wire   [14:0] trunc_ln43_10_fu_80_p1;
wire   [14:0] datareg_7_fu_112_p3;
wire   [0:0] icmp_ln45_8_fu_124_p2;
wire   [14:0] trunc_ln43_9_fu_76_p1;
wire   [14:0] datareg_8_fu_130_p3;
wire   [0:0] icmp_ln45_9_fu_142_p2;
wire   [14:0] trunc_ln43_8_fu_72_p1;
wire   [14:0] datareg_9_fu_148_p3;
wire   [0:0] icmp_ln45_10_fu_160_p2;
wire   [14:0] trunc_ln43_7_fu_68_p1;
wire   [14:0] datareg_10_fu_166_p3;
wire   [0:0] icmp_ln45_11_fu_178_p2;
wire   [14:0] trunc_ln43_fu_64_p1;
wire   [14:0] datareg_11_fu_184_p3;
wire   [15:0] zext_ln45_fu_102_p1;
wire   [15:0] zext_ln45_7_fu_120_p1;
wire   [15:0] zext_ln45_8_fu_138_p1;
wire   [15:0] zext_ln45_9_fu_156_p1;
wire   [15:0] zext_ln45_10_fu_174_p1;
wire   [15:0] zext_ln45_11_fu_192_p1;

assign ap_ready = 1'b1;

assign datareg_10_fu_166_p3 = ((icmp_ln45_10_fu_160_p2[0:0] == 1'b1) ? trunc_ln43_7_fu_68_p1 : 15'd0);

assign datareg_11_fu_184_p3 = ((icmp_ln45_11_fu_178_p2[0:0] == 1'b1) ? trunc_ln43_fu_64_p1 : 15'd0);

assign datareg_7_fu_112_p3 = ((icmp_ln45_7_fu_106_p2[0:0] == 1'b1) ? trunc_ln43_10_fu_80_p1 : 15'd0);

assign datareg_8_fu_130_p3 = ((icmp_ln45_8_fu_124_p2[0:0] == 1'b1) ? trunc_ln43_9_fu_76_p1 : 15'd0);

assign datareg_9_fu_148_p3 = ((icmp_ln45_9_fu_142_p2[0:0] == 1'b1) ? trunc_ln43_8_fu_72_p1 : 15'd0);

assign datareg_fu_94_p3 = ((icmp_ln45_fu_88_p2[0:0] == 1'b1) ? trunc_ln43_11_fu_84_p1 : 15'd0);

assign trunc_ln43_10_fu_80_p1 = data_1_val[14:0];

assign trunc_ln43_11_fu_84_p1 = data_0_val[14:0];

assign trunc_ln43_7_fu_68_p1 = data_4_val[14:0];

assign trunc_ln43_8_fu_72_p1 = data_3_val[14:0];

assign trunc_ln43_9_fu_76_p1 = data_2_val[14:0];

assign trunc_ln43_fu_64_p1 = data_5_val[14:0];

assign zext_ln45_10_fu_174_p1 = datareg_10_fu_166_p3;

assign zext_ln45_11_fu_192_p1 = datareg_11_fu_184_p3;

assign zext_ln45_7_fu_120_p1 = datareg_7_fu_112_p3;

assign zext_ln45_8_fu_138_p1 = datareg_8_fu_130_p3;

assign zext_ln45_9_fu_156_p1 = datareg_9_fu_148_p3;

assign zext_ln45_fu_102_p1 = datareg_fu_94_p3;

assign ap_return_0 = zext_ln45_fu_102_p1;

assign ap_return_1 = zext_ln45_7_fu_120_p1;

assign ap_return_2 = zext_ln45_8_fu_138_p1;

assign ap_return_3 = zext_ln45_9_fu_156_p1;

assign ap_return_4 = zext_ln45_10_fu_174_p1;

assign ap_return_5 = zext_ln45_11_fu_192_p1;

assign icmp_ln45_10_fu_160_p2 = (($signed(data_4_val) > $signed(16'd0)) ? 1'b1 : 1'b0);

assign icmp_ln45_11_fu_178_p2 = (($signed(data_5_val) > $signed(16'd0)) ? 1'b1 : 1'b0);

assign icmp_ln45_7_fu_106_p2 = (($signed(data_1_val) > $signed(16'd0)) ? 1'b1 : 1'b0);

assign icmp_ln45_8_fu_124_p2 = (($signed(data_2_val) > $signed(16'd0)) ? 1'b1 : 1'b0);

assign icmp_ln45_9_fu_142_p2 = (($signed(data_3_val) > $signed(16'd0)) ? 1'b1 : 1'b0);

assign icmp_ln45_fu_88_p2 = (($signed(data_0_val) > $signed(16'd0)) ? 1'b1 : 1'b0);

endmodule //myproject_relu_ap_fixed_16_6_5_3_0_ap_fixed_16_6_5_3_0_relu_config7_s
