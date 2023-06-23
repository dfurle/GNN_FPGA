// ==============================================================
// Generated by Vitis HLS v2023.1
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
// ==============================================================

`timescale 1 ns / 1 ps 

module myproject_dense_latency_ap_fixed_16_6_5_3_0_ap_fixed_16_6_5_3_0_config4_s (
        ap_ready,
        data_4_val,
        data_5_val,
        data_7_val,
        ap_return_0,
        ap_return_1,
        ap_return_2,
        ap_return_3,
        ap_return_4,
        ap_return_5
);


output   ap_ready;
input  [15:0] data_4_val;
input  [15:0] data_5_val;
input  [15:0] data_7_val;
output  [15:0] ap_return_0;
output  [15:0] ap_return_1;
output  [15:0] ap_return_2;
output  [15:0] ap_return_3;
output  [15:0] ap_return_4;
output  [15:0] ap_return_5;

wire  signed [10:0] mul_ln42_20_fu_81_p1;
wire  signed [15:0] mul_ln42_19_fu_86_p0;
wire  signed [25:0] sext_ln70_fu_452_p1;
wire   [10:0] mul_ln42_19_fu_86_p1;
wire  signed [15:0] mul_ln42_fu_89_p0;
wire   [10:0] mul_ln42_fu_89_p1;
wire  signed [9:0] mul_ln73_4_fu_91_p1;
wire  signed [15:0] mul_ln73_fu_93_p0;
wire   [9:0] mul_ln73_fu_93_p1;
wire  signed [15:0] sext_ln70_fu_452_p0;
wire   [25:0] mul_ln73_fu_93_p2;
wire  signed [15:0] shl_ln_fu_469_p1;
wire   [24:0] shl_ln_fu_469_p3;
wire  signed [25:0] sext_ln73_fu_477_p1;
wire  signed [15:0] shl_ln73_2_fu_487_p1;
wire   [20:0] shl_ln73_2_fu_487_p3;
wire   [25:0] sub_ln73_fu_481_p2;
wire  signed [25:0] sext_ln73_5_fu_495_p1;
wire   [25:0] sub_ln73_2_fu_499_p2;
wire   [25:0] mul_ln42_fu_89_p2;
wire   [25:0] mul_ln42_19_fu_86_p2;
wire   [25:0] mul_ln73_4_fu_91_p2;
wire   [25:0] mul_ln42_20_fu_81_p2;
wire   [15:0] trunc_ln_fu_459_p4;
wire   [15:0] trunc_ln42_s_fu_505_p4;
wire   [15:0] trunc_ln42_23_fu_515_p4;
wire   [15:0] trunc_ln42_24_fu_525_p4;
wire   [15:0] trunc_ln42_25_fu_540_p4;
wire   [15:0] trunc_ln42_26_fu_555_p4;
wire   [15:0] add_ln58_fu_565_p2;
wire   [15:0] add_ln58_22_fu_571_p2;
wire   [15:0] add_ln58_23_fu_577_p2;
wire   [15:0] add_ln58_24_fu_583_p2;
wire   [15:0] add_ln58_25_fu_589_p2;
wire   [15:0] add_ln58_26_fu_595_p2;

myproject_mul_16s_11s_26_1_1 #(
    .ID( 1 ),
    .NUM_STAGE( 1 ),
    .din0_WIDTH( 16 ),
    .din1_WIDTH( 11 ),
    .dout_WIDTH( 26 ))
mul_16s_11s_26_1_1_U10(
    .din0(data_7_val),
    .din1(mul_ln42_20_fu_81_p1),
    .dout(mul_ln42_20_fu_81_p2)
);

myproject_mul_16s_11ns_26_1_1 #(
    .ID( 1 ),
    .NUM_STAGE( 1 ),
    .din0_WIDTH( 16 ),
    .din1_WIDTH( 11 ),
    .dout_WIDTH( 26 ))
mul_16s_11ns_26_1_1_U11(
    .din0(mul_ln42_19_fu_86_p0),
    .din1(mul_ln42_19_fu_86_p1),
    .dout(mul_ln42_19_fu_86_p2)
);

myproject_mul_16s_11ns_26_1_1 #(
    .ID( 1 ),
    .NUM_STAGE( 1 ),
    .din0_WIDTH( 16 ),
    .din1_WIDTH( 11 ),
    .dout_WIDTH( 26 ))
mul_16s_11ns_26_1_1_U12(
    .din0(mul_ln42_fu_89_p0),
    .din1(mul_ln42_fu_89_p1),
    .dout(mul_ln42_fu_89_p2)
);

myproject_mul_16s_10s_26_1_1 #(
    .ID( 1 ),
    .NUM_STAGE( 1 ),
    .din0_WIDTH( 16 ),
    .din1_WIDTH( 10 ),
    .dout_WIDTH( 26 ))
mul_16s_10s_26_1_1_U13(
    .din0(data_5_val),
    .din1(mul_ln73_4_fu_91_p1),
    .dout(mul_ln73_4_fu_91_p2)
);

myproject_mul_16s_10ns_26_1_1 #(
    .ID( 1 ),
    .NUM_STAGE( 1 ),
    .din0_WIDTH( 16 ),
    .din1_WIDTH( 10 ),
    .dout_WIDTH( 26 ))
mul_16s_10ns_26_1_1_U14(
    .din0(mul_ln73_fu_93_p0),
    .din1(mul_ln73_fu_93_p1),
    .dout(mul_ln73_fu_93_p2)
);

assign add_ln58_22_fu_571_p2 = (trunc_ln42_s_fu_505_p4 + 16'd15);

assign add_ln58_23_fu_577_p2 = (trunc_ln42_23_fu_515_p4 + 16'd17);

assign add_ln58_24_fu_583_p2 = ($signed(trunc_ln42_24_fu_525_p4) + $signed(16'd65528));

assign add_ln58_25_fu_589_p2 = ($signed(trunc_ln42_25_fu_540_p4) + $signed(16'd65527));

assign add_ln58_26_fu_595_p2 = (trunc_ln42_26_fu_555_p4 + 16'd7);

assign add_ln58_fu_565_p2 = ($signed(trunc_ln_fu_459_p4) + $signed(16'd65535));

assign ap_ready = 1'b1;

assign sext_ln70_fu_452_p1 = sext_ln70_fu_452_p0;

assign sext_ln73_5_fu_495_p1 = $signed(shl_ln73_2_fu_487_p3);

assign sext_ln73_fu_477_p1 = $signed(shl_ln_fu_469_p3);

assign shl_ln73_2_fu_487_p3 = {{shl_ln73_2_fu_487_p1}, {5'd0}};

assign shl_ln_fu_469_p3 = {{shl_ln_fu_469_p1}, {9'd0}};

assign sub_ln73_2_fu_499_p2 = ($signed(sub_ln73_fu_481_p2) - $signed(sext_ln73_5_fu_495_p1));

assign sub_ln73_fu_481_p2 = ($signed(26'd0) - $signed(sext_ln73_fu_477_p1));

assign trunc_ln42_23_fu_515_p4 = {{mul_ln42_fu_89_p2[25:10]}};

assign trunc_ln42_24_fu_525_p4 = {{mul_ln42_19_fu_86_p2[25:10]}};

assign trunc_ln42_25_fu_540_p4 = {{mul_ln73_4_fu_91_p2[25:10]}};

assign trunc_ln42_26_fu_555_p4 = {{mul_ln42_20_fu_81_p2[25:10]}};

assign trunc_ln42_s_fu_505_p4 = {{sub_ln73_2_fu_499_p2[25:10]}};

assign trunc_ln_fu_459_p4 = {{mul_ln73_fu_93_p2[25:10]}};

assign ap_return_0 = add_ln58_fu_565_p2;

assign ap_return_1 = add_ln58_22_fu_571_p2;

assign ap_return_2 = add_ln58_23_fu_577_p2;

assign ap_return_3 = add_ln58_24_fu_583_p2;

assign ap_return_4 = add_ln58_25_fu_589_p2;

assign ap_return_5 = add_ln58_26_fu_595_p2;

assign mul_ln42_19_fu_86_p0 = sext_ln70_fu_452_p1;

assign mul_ln42_19_fu_86_p1 = 26'd551;

assign mul_ln42_20_fu_81_p1 = 26'd67108246;

assign mul_ln42_fu_89_p0 = sext_ln70_fu_452_p1;

assign mul_ln42_fu_89_p1 = 26'd517;

assign mul_ln73_4_fu_91_p1 = 26'd67108397;

assign mul_ln73_fu_93_p0 = sext_ln70_fu_452_p1;

assign mul_ln73_fu_93_p1 = 26'd467;

assign sext_ln70_fu_452_p0 = data_4_val;

assign shl_ln73_2_fu_487_p1 = data_4_val;

assign shl_ln_fu_469_p1 = data_4_val;

endmodule //myproject_dense_latency_ap_fixed_16_6_5_3_0_ap_fixed_16_6_5_3_0_config4_s
