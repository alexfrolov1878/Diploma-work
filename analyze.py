import re
import xlsxwriter


NUM_TESTS = 25
RUNS = (1, 2, 3, 4)
MEM_TYPES = ('standard', 'memory_absolute', 'memory_relative', 'memory_forgetting')
CR_TYPES = (1, 2, 3, 4, 5)
CR_STRS = (
	'Standard one-point crossover',
	'One-point crossover with swapping',
	'One-point crossover with copying', 
	'Uniform crossover with swapping',
	'Uniform crossover with copying'
)
CHART_POS = ('C3', 'C28', 'C53', 'C78', 'C103')
FEATURES = ('precision', 'time')


def construct_name(mem, cr):
	return str(mem) + '_' + str(cr)


def parse_data(dir):
	data = {}
	patterns = {}
	patterns['precision'] = re.compile(r"Result is (\d+\.\d+|\d+)")
	patterns['time'] = re.compile(r"It took (\d+\.\d+|\d+)")

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
	
	data[0] = {}
	# compute averages
	for mem in MEM_TYPES:
		data[0][mem] = {}
		for cr in CR_TYPES:
			data[0][mem][cr] = {}
			for feat in FEATURES:
				data[0][mem][cr][feat] = [0.0] * NUM_TESTS
				for i in range(NUM_TESTS):
					s = 0.0
					for run in RUNS:
						s += eval(data[run][mem][cr][feat][i]);
					data[0][mem][cr][feat][i] = s / len(RUNS);
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

	# fill in data over runs
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

	# fill in average data
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
			for item in data[0][mem][cr][feat]:
				worksheet.write(row, col, item, num_cell)
				col += 1
			row += 1
	return


def tune_chart(chart, feat, title):
	chart.set_size({
		'width': 720,
		'height': 480
	})
	chart.set_title({
		'name': title
	})
	chart.set_chartarea({
		'border': {'none': True},
		'fill':   {'color': 'white'}
	})
	chart.set_legend({
		'position': 'bottom',
		'font': {'size': 9, 'bold': 1}
	})
	chart.set_x_axis({
		'name': 'tests',
		'name_font': {'rotation': 0, 'size': 12, 'bold': False},
		'num_font':  {'italic': True },
		'major_gridlines': {
			'visible': True,
			'line': {'width': 0.5, 'dash_type': 'dash'}
		}
	})
	chart.set_y_axis({
		'name': feat,
		'name_font': {'rotation': -89, 'size': 12, 'bold': False},
		'num_font':  {'italic': True },
	})
	return

def plot_chart(chart, worksheet, data, feat, cr, tmp_row):
	for mem in MEM_TYPES:
		# skip standard algorithm as it is the base level
		if mem == 'standard':
			continue

		# compute the difference row
		for test_num in range(NUM_TESTS):
			if feat == 'precision':
				# absolute difference for precision
				diff = data[0]['standard'][cr][feat][test_num] - \
					data[0][mem][cr][feat][test_num]
			else:
				# percents for time
				diff = 100.0 * data[0]['standard'][cr][feat][test_num] / \
					data[0][mem][cr][feat][test_num] - 100.0
			worksheet.write(tmp_row, test_num + 1, diff)
		tmp_row += 1
		
		# add difference series
		chart.add_series({
			'name': mem,
			'values': '=' + feat + '!$B$' + str(tmp_row) + ':$Z$' + str(tmp_row),
			'marker': {'type': 'diamond'},
		})
		
	return chart


def plot_data(workbook, worksheet, data, feat):
	tmp_row = 150
	for cr in CR_TYPES:
		chart = workbook.add_chart({'type': 'column'})
		tune_chart(chart, feat, CR_STRS[cr - 1])
		plot_chart(chart, worksheet, data, feat, cr, tmp_row)
		worksheet.insert_chart(CHART_POS[cr - 1], chart)
		tmp_row += len(MEM_TYPES)
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
		plot_data(workbook, worksheet, data, feat)
	workbook.close()
	print '   Done!'


def main():
	analyze_data('job_scheduling')
	analyze_data('subset_sum')


if __name__ == '__main__':
	main()