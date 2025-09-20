module simple_regfile_alu_tb;

    parameter REG_WIDTH = 64;
    parameter REG_COUNT = 32;
    parameter ADDR_WIDTH = 5;

    reg clk;
    reg [ADDR_WIDTH-1:0] inst_rs1;
    reg [ADDR_WIDTH-1:0] inst_rs2;
    reg [ADDR_WIDTH-1:0] inst_rd;
    reg inst_immflag;
    reg [REG_WIDTH-1:0] imm_data;
    wire [REG_WIDTH-1:0] alu_result;

    // Instantiate the DUT
    simple_regfile_alu #(
        .REG_WIDTH(REG_WIDTH),
        .REG_COUNT(REG_COUNT),
        .ADDR_WIDTH(ADDR_WIDTH)
    ) dut (
        .clk(clk),
        .inst_rs1(inst_rs1),
        .inst_rs2(inst_rs2),
        .inst_rd(inst_rd),
        .inst_immflag(inst_immflag),
        .imm_data(imm_data),
        .alu_result(alu_result)
    );

    // Clock generation: toggle every 5 time units
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    integer i;

    // Preload some register values by directly writing to regfile via force (since regfile is internal)
    // Alternatively, we can simulate initial conditions by applying instructions that write known values.
    // Since regfile is internal and no direct access, we'll simulate by writing immediate values.

    initial begin
        // Initialize inputs
        inst_rs1 = 0;
        inst_rs2 = 0;
        inst_rd = 0;
        inst_immflag = 0;
        imm_data = 0;

        // Wait for a couple of clock cycles
        @(posedge clk);
        @(posedge clk);

        // Write 10 to regfile[1] using immediate flag
        inst_rs1 = 0;          // rs1 = regfile[0] (assumed zero)
        inst_rs2 = 0;          // rs2 = regfile[0]
        inst_rd = 1;           // write to regfile[1]
        inst_immflag = 1;      // use immediate
        imm_data = 10;         // immediate value 10
        @(posedge clk);
        @(negedge clk);        // wait for writeback

        // Write 20 to regfile[2] using immediate flag
        inst_rs1 = 0;
        inst_rs2 = 0;
        inst_rd = 2;
        inst_immflag = 1;
        imm_data = 20;
        @(posedge clk);
        @(negedge clk);

        // Add regfile[1] + regfile[2], write result to regfile[3], no immediate
        inst_rs1 = 1;
        inst_rs2 = 2;
        inst_rd = 3;
        inst_immflag = 0;
        imm_data = 0;
        @(posedge clk);
        @(negedge clk);

        // Add regfile[3] + immediate 5, write to regfile[4]
        inst_rs1 = 3;
        inst_rs2 = 0;
        inst_rd = 4;
        inst_immflag = 1;
        imm_data = 5;
        @(posedge clk);
        @(negedge clk);
		#1
        // Print contents of the entire regfile
        for (i = 0; i < REG_COUNT; i = i + 1) begin
            $display("Register %0d = %0d", i, dut.regfile[i]);
        end
        $finish;

    end

    // Monitor outputs
    initial begin
        $monitor("Time=%0t clk=%b rs1=%0d rs2=%0d rd=%0d immflag=%b imm_data=%0d alu_result=%0d",
                 $time, clk, inst_rs1, inst_rs2, inst_rd, inst_immflag, imm_data, alu_result);
    end

    always @(negedge clk) begin
        $display("Time=%0t: Register %0d written with value %0d", $time, dut.inst_rd, dut.regfile[dut.inst_rd]);
    end

    // Final block to finish simulation after printing regfile contents
    initial begin
        @(negedge clk);
        #1;
    end

endmodule