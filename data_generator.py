import csv
import random
from datetime import datetime, timedelta


male_first = ['Иван', 'Петр', 'Алексей', 'Дмитрий', 'Сергей', 'Виктор', 'Андрей', 'Богдан', 'Антон', 'Марк']
male_last = ['Иванов', 'Петров', 'Сидоров', 'Кузнецов', 'Смирнов', 'Будейко', 'Викторов', 'Богданов', 'Антонов', 'Марков']
male_middle = ['Иванович', 'Петрович', 'Алексеевич', 'Дмитриевич', 'Сергеевич', 'Викторович', 'Андреевич', 'Богданович', 'Антонович', 'Маркович']

female_first = ['Анна', 'Мария', 'Елена', 'Ольга', 'Татьяна', 'Марина', 'Кристина', 'Ангелина', 'Ксения', 'Юлия']
female_last = ['Иванова', 'Петрова', 'Сидорова', 'Кузнецова', 'Смирнова', 'Будейко', 'Викторова', 'Богданова', 'Антонова', 'Маркова']
female_middle = ['Ивановна', 'Петровна', 'Алексеевна', 'Дмитриевна', 'Сергеевна', 'Викторовна', 'Андреевна', 'Богдановна', 'Антоновна', 'Марковна']


def gen_fio(is_male):
    firsts = male_first if is_male else female_first
    lasts = male_last if is_male else female_last
    middle = male_middle if is_male else female_middle
    return f'{random.choice(lasts)} {random.choice(firsts)} {random.choice(middle)}'

sizes = [5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000]
zagss = list(range(1, 101))

for size in sizes:
    print(f"Generating zags_{size}.csv")
    with open(f'data/zags_{size}.csv', 'w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow(['groom_fio', 'groom_birthday', 'bride_fio', 'bride_birthday', 'wedding_date', 'zags_number'])
        base = datetime(1990, 1, 1)
        for _ in range(size):
            groom = gen_fio(True)
            bride = gen_fio(False)
            groom_birthday = (base + timedelta(days=random.randint(0, 10000))).strftime('%d.%m.%Y')
            bride_birthday = (base + timedelta(days=random.randint(0, 10000))).strftime('%d.%m.%Y')
            wedding_day = (base + timedelta(days=random.randint(15000, 25000))).strftime('%d.%m.%Y')
            zags = random.choice(zagss)
            writer.writerow([groom, groom_birthday, bride, bride_birthday, wedding_day, zags])
