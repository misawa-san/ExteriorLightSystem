
import sys
import csv
from pathlib import Path

def conv_csv(in_name, out_name):
    try:
        in_file = open(in_name, 'r', newline='')
    except OSError as e:
        print(e)
        return

    try:
        out_file = open(out_name, 'w', newline='')
    except OSError as e:
        print(e)
        in_file.close
        return

    try:
        reader = csv.reader(in_file)
        header = next(reader)
        writer = csv.writer(out_file, delimiter=' ')
    except OSError as e:
        print(e)
    else:
        for row in reader:
            for index, col in enumerate(row[1:], 1):
                if col == 'TRUE':
                    val = '1'
                elif row[index] == 'FALSE':
                    val = '0'
                else:
                    val = col
                writer.writerow([f'{float(row[0])*1000:.0f}',header[index],val,'f'])
    finally:
        in_file.close
        out_file.close
        return

arg = sys.argv
if len(arg) > 1:
    in_path = Path(arg[1])
    if Path.is_file(in_path):
        if in_path.suffix == '.csv':
            conv_csv(arg[1], str(in_path.parent) + '/output_' + str(in_path.name))
        else:
            print('csvファイルではありません')
    elif Path.is_dir(in_path):
        out_path = Path(in_path.parent, 'output_' + str(in_path.name))
        if out_path.exists():
            print('出力用フォルダが既にあります')
        else:
            out_path.mkdir()
            for file_or_dir in in_path.iterdir():
                if file_or_dir.is_file() and file_or_dir.suffix == '.csv':
                    conv_csv(file_or_dir, str(out_path) + '/output_' + str(file_or_dir.name))
    else:
        print('存在しないフォルダ・ファイルです')
else:
    print('引数が指定されていません')
