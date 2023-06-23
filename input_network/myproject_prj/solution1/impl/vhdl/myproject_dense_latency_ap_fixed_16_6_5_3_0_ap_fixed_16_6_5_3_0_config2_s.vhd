-- ==============================================================
-- Generated by Vitis HLS v2023.1
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
-- ==============================================================

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity myproject_dense_latency_ap_fixed_16_6_5_3_0_ap_fixed_16_6_5_3_0_config2_s is
port (
    ap_ready : OUT STD_LOGIC;
    data_val : IN STD_LOGIC_VECTOR (31 downto 0);
    ap_return_0 : OUT STD_LOGIC_VECTOR (15 downto 0);
    ap_return_1 : OUT STD_LOGIC_VECTOR (15 downto 0);
    ap_return_2 : OUT STD_LOGIC_VECTOR (15 downto 0) );
end;


architecture behav of myproject_dense_latency_ap_fixed_16_6_5_3_0_ap_fixed_16_6_5_3_0_config2_s is 
    constant ap_const_logic_1 : STD_LOGIC := '1';
    constant ap_const_boolean_1 : BOOLEAN := true;
    constant ap_const_lv26_309 : STD_LOGIC_VECTOR (25 downto 0) := "00000000000000001100001001";
    constant ap_const_lv26_3FFFDAD : STD_LOGIC_VECTOR (25 downto 0) := "11111111111111110110101101";
    constant ap_const_lv26_313 : STD_LOGIC_VECTOR (25 downto 0) := "00000000000000001100010011";
    constant ap_const_lv32_A : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000001010";
    constant ap_const_lv32_19 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000011001";
    constant ap_const_lv32_10 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000010000";
    constant ap_const_lv32_1F : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000011111";
    constant ap_const_lv16_3 : STD_LOGIC_VECTOR (15 downto 0) := "0000000000000011";
    constant ap_const_lv16_FFFC : STD_LOGIC_VECTOR (15 downto 0) := "1111111111111100";
    constant ap_const_lv16_FFF4 : STD_LOGIC_VECTOR (15 downto 0) := "1111111111110100";
    constant ap_const_logic_0 : STD_LOGIC := '0';

attribute shreg_extract : string;
    signal mul_ln42_fu_50_p0 : STD_LOGIC_VECTOR (15 downto 0);
    signal sext_ln73_fu_128_p1 : STD_LOGIC_VECTOR (25 downto 0);
    signal mul_ln42_fu_50_p1 : STD_LOGIC_VECTOR (10 downto 0);
    signal mul_ln42_21_fu_52_p0 : STD_LOGIC_VECTOR (15 downto 0);
    signal mul_ln42_21_fu_52_p1 : STD_LOGIC_VECTOR (10 downto 0);
    signal mul_ln42_22_fu_53_p1 : STD_LOGIC_VECTOR (10 downto 0);
    signal a_fu_124_p1 : STD_LOGIC_VECTOR (15 downto 0);
    signal mul_ln42_fu_50_p2 : STD_LOGIC_VECTOR (25 downto 0);
    signal mul_ln42_21_fu_52_p2 : STD_LOGIC_VECTOR (25 downto 0);
    signal a_1_fu_154_p4 : STD_LOGIC_VECTOR (15 downto 0);
    signal mul_ln42_22_fu_53_p2 : STD_LOGIC_VECTOR (25 downto 0);
    signal trunc_ln_fu_134_p4 : STD_LOGIC_VECTOR (15 downto 0);
    signal trunc_ln42_s_fu_144_p4 : STD_LOGIC_VECTOR (15 downto 0);
    signal trunc_ln42_27_fu_169_p4 : STD_LOGIC_VECTOR (15 downto 0);
    signal add_ln58_28_fu_191_p2 : STD_LOGIC_VECTOR (15 downto 0);
    signal add_ln58_fu_179_p2 : STD_LOGIC_VECTOR (15 downto 0);
    signal add_ln58_27_fu_185_p2 : STD_LOGIC_VECTOR (15 downto 0);

    component myproject_mul_16s_11ns_26_1_1 IS
    generic (
        ID : INTEGER;
        NUM_STAGE : INTEGER;
        din0_WIDTH : INTEGER;
        din1_WIDTH : INTEGER;
        dout_WIDTH : INTEGER );
    port (
        din0 : IN STD_LOGIC_VECTOR (15 downto 0);
        din1 : IN STD_LOGIC_VECTOR (10 downto 0);
        dout : OUT STD_LOGIC_VECTOR (25 downto 0) );
    end component;


    component myproject_mul_16s_11s_26_1_1 IS
    generic (
        ID : INTEGER;
        NUM_STAGE : INTEGER;
        din0_WIDTH : INTEGER;
        din1_WIDTH : INTEGER;
        dout_WIDTH : INTEGER );
    port (
        din0 : IN STD_LOGIC_VECTOR (15 downto 0);
        din1 : IN STD_LOGIC_VECTOR (10 downto 0);
        dout : OUT STD_LOGIC_VECTOR (25 downto 0) );
    end component;



begin
    mul_16s_11ns_26_1_1_U1 : component myproject_mul_16s_11ns_26_1_1
    generic map (
        ID => 1,
        NUM_STAGE => 1,
        din0_WIDTH => 16,
        din1_WIDTH => 11,
        dout_WIDTH => 26)
    port map (
        din0 => mul_ln42_fu_50_p0,
        din1 => mul_ln42_fu_50_p1,
        dout => mul_ln42_fu_50_p2);

    mul_16s_11s_26_1_1_U2 : component myproject_mul_16s_11s_26_1_1
    generic map (
        ID => 1,
        NUM_STAGE => 1,
        din0_WIDTH => 16,
        din1_WIDTH => 11,
        dout_WIDTH => 26)
    port map (
        din0 => mul_ln42_21_fu_52_p0,
        din1 => mul_ln42_21_fu_52_p1,
        dout => mul_ln42_21_fu_52_p2);

    mul_16s_11ns_26_1_1_U3 : component myproject_mul_16s_11ns_26_1_1
    generic map (
        ID => 1,
        NUM_STAGE => 1,
        din0_WIDTH => 16,
        din1_WIDTH => 11,
        dout_WIDTH => 26)
    port map (
        din0 => a_1_fu_154_p4,
        din1 => mul_ln42_22_fu_53_p1,
        dout => mul_ln42_22_fu_53_p2);




    a_1_fu_154_p4 <= data_val(31 downto 16);
    a_fu_124_p1 <= data_val(16 - 1 downto 0);
    add_ln58_27_fu_185_p2 <= std_logic_vector(unsigned(trunc_ln42_s_fu_144_p4) + unsigned(ap_const_lv16_FFFC));
    add_ln58_28_fu_191_p2 <= std_logic_vector(unsigned(trunc_ln42_27_fu_169_p4) + unsigned(ap_const_lv16_FFF4));
    add_ln58_fu_179_p2 <= std_logic_vector(unsigned(trunc_ln_fu_134_p4) + unsigned(ap_const_lv16_3));
    ap_ready <= ap_const_logic_1;
    ap_return_0 <= add_ln58_28_fu_191_p2;
    ap_return_1 <= add_ln58_fu_179_p2;
    ap_return_2 <= add_ln58_27_fu_185_p2;
    mul_ln42_21_fu_52_p0 <= sext_ln73_fu_128_p1(16 - 1 downto 0);
    mul_ln42_21_fu_52_p1 <= ap_const_lv26_3FFFDAD(11 - 1 downto 0);
    mul_ln42_22_fu_53_p1 <= ap_const_lv26_313(11 - 1 downto 0);
    mul_ln42_fu_50_p0 <= sext_ln73_fu_128_p1(16 - 1 downto 0);
    mul_ln42_fu_50_p1 <= ap_const_lv26_309(11 - 1 downto 0);
        sext_ln73_fu_128_p1 <= std_logic_vector(IEEE.numeric_std.resize(signed(a_fu_124_p1),26));

    trunc_ln42_27_fu_169_p4 <= mul_ln42_22_fu_53_p2(25 downto 10);
    trunc_ln42_s_fu_144_p4 <= mul_ln42_21_fu_52_p2(25 downto 10);
    trunc_ln_fu_134_p4 <= mul_ln42_fu_50_p2(25 downto 10);
end behav;
