local bufnr = 4

vim.api.nvim_create_autocmd("BufWritePost", {
	group = vim.api.nvim_create_augroup("Markos G1", { clear = true }),
	pattern = "main.cc",
	callback = function()
		vim.fn.jobstart({"gcc", "-std=c++20", "-masm=intel", "-S", "main.cc", "-o", "-"}, {
			stdout_buffered = true,
			on_stdout = function(_, data)
				if data then
					vim.api.nvim_buf_set_lines(bufnr, 0, 0, false, data)
				end
			end,
			on_stderr = function(_, data)
				if data then
					vim.api.nvim_buf_set_lines(bufnr, 0, 0, false, data)
				end
			end
		})
	end,
})
