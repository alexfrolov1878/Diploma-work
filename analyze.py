import re
import xlsxwriter as xlsxwriter


NUM_TESTS = 25
RUNS = (1, 2, 3, 4)
MEM_TYPES = ('standard', 'memory_absolute', 'memory_relative', 'memory_forgetting')
CR_TYPES = (1, 2, 3, 4, 5)
FEATURES = ('precision', 'time', 'iterations')


def construct_name(mem, cr):
	return str(mem) + '_' + str(cr)


def parse_data(dir):
	data = {}
	patterns = {}
	patterns['precision'] = re.compile(r"Result is (\d+\.\d+|\d+)")
	patterns['time'] = re.compile(r"It took (\d+\.\d+|\d+)")
	patterns['iterations'] = re.compile(r"Total number of iterations: (\d+)")

	for run in RUNS:
		data[run] = {}
		for mem in MEM_TYPES:
			data[run][mem] = {}
			for cr in CR_TYPES:
				data[run][mem][cr] = {}
				filename = dir + str(run) + '/' + construct_name(mem, cr) + '.txt'
				with open(filename) as f:
					for feat in FEATURES:
						data[run][mem][cr][feat] = ()
					for line in f:
						for feat in FEATURES:
							match = patterns[feat].match(line)
							if match != None:
								data[run][mem][cr][feat] += match.groups()
	return data


def write_data(workbook, worksheet, data, feat):
	height = len(MEM_TYPES) * len(CR_TYPES) + 1
	spaces = 4
	
	# style formats
	header_cell = workbook.add_format({'bold': True, 'align': 'center'})
	header_cell.set_border(7)
	test_cell = workbook.add_format({'italic': True, 'align': 'center'})
	test_cell.set_bottom(7)
	alg_cell = workbook.add_format({'italic': True, 'align': 'center'})
	alg_cell.set_right(7)
	num_cell = workbook.add_format({'num_format': '0.000', 'align': 'center'})

	# fill in data
	for run in RUNS:
		row = run * (height + spaces)
		col = 0
		worksheet.write(row, col, 'RUN #' + str(run), header_cell)
		col += 1
		for test_num in range(NUM_TESTS):
			worksheet.write(row, col, test_num + 1, test_cell)
			col += 1
		row += 1
		for mem in MEM_TYPES:
			for cr in CR_TYPES:
				col = 0
				name = construct_name(mem, cr)
				worksheet.write(row, col, name, alg_cell)
				col += 1
				for item in data[run][mem][cr][feat]:
					worksheet.write(row, col, item, num_cell)
					col += 1
				row += 1

	# compute averages
	row = 0
	col = 0
	worksheet.write(row, col, 'AVERAGE', header_cell)
	col += 1
	for test_num in range(NUM_TESTS):
		worksheet.write(row, col, test_num + 1, test_cell)
		col += 1
	row += 1
	for mem in MEM_TYPES:
		for cr in CR_TYPES:
			col = 0
			name = construct_name(mem, cr)
			worksheet.write(row, col, name, alg_cell)
			col += 1
			for i in range(NUM_TESTS):
				s = 0.0;
				for run in RUNS:
					s += eval(data[run][mem][cr][feat][i]);
				s /= len(RUNS);
				worksheet.write(row, col, s, num_cell)
				col += 1
			row += 1
	return


def analyze_data(algorithm):
	print ' Parsing data for ' + algorithm + ' algorithm...'
	data = parse_data(algorithm + '/results/')
	print '  Creating ' + algorithm + '.xlsx...'
	workbook = xlsxwriter.Workbook(algorithm + '.xlsx')
	for feat in FEATURES:
		worksheet = workbook.add_worksheet(feat)
		worksheet.set_column(0 , 0, 20)
		worksheet.set_column(1, NUM_TESTS + 1, 12)
		write_data(workbook, worksheet, data, feat)
	workbook.close()
	print '   Done!'


def main():
	analyze_data('job_scheduling')
	analyze_data('subset_sum')


if __name__ == '__main__':
	main()