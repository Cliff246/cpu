module simple_regfile_alu #(
    parameter REG_WIDTH = 64,
    parameter REG_COUNT = 32,
    parameter ADDR_WIDTH = 5
) (
    input  clk,
    input  [ADDR_WIDTH-1:0] inst_rs1,
    input  [ADDR_WIDTH-1:0] inst_rs2,
    input  [ADDR_WIDTH-1:0] inst_rd,
    input  inst_immflag,
    input  [REG_WIDTH-1:0] imm_data,
    output reg [REG_WIDTH-1:0] alu_result
);
    // Register file
    reg [REG_WIDTH-1:0] regfile [0:REG_COUNT-1];

    integer i;
    initial begin
        for (i = 0; i < REG_COUNT; i = i + 1) begin
            regfile[i] = 0;
        end
    end

    reg [ADDR_WIDTH-1:0] rs1_sel, rs2_sel, rd_sel;
    reg immflag_latched;
    reg [REG_WIDTH-1:0] imm_latched;

    always @(posedge clk) begin
        rs1_sel <= inst_rs1;
        rs2_sel <= inst_rs2;
        rd_sel  <= inst_rd;
        immflag_latched <= inst_immflag;
        imm_latched <= imm_data;
    end

    // Combinational: regfile read
    wire [REG_WIDTH-1:0] rs1_data = regfile[rs1_sel];
    wire [REG_WIDTH-1:0] rs2_data = regfile[rs2_sel];
    reg [REG_WIDTH-1:0] computed;

    // Falling edge: compute ALU result and writeback
    always @(negedge clk) begin
       	computed = rs1_data + rs2_data + (immflag_latched ? imm_latched : {REG_WIDTH{1'b0}});
        if (rd_sel != {ADDR_WIDTH{1'b0}})
            regfile[rd_sel] <= computed;
        alu_result <= computed;
    end

endmodule