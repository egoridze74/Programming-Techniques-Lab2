import pandas as pd
import matplotlib.pyplot as plt


df = pd.read_csv('search_times.csv')

plt.figure(figsize=(14, 10))

# График 1: Время поиска (логарифмический масштаб)
plt.subplot(2, 1, 1)
plt.plot(df['size'], df['linear_ms'], label='Linear Search (O(n))', marker='o', linewidth=2, markersize=4)
plt.plot(df['size'], df['bintree_ms'], label='BinTree (O(log n) avg)', marker='s', linewidth=2, markersize=4)
plt.plot(df['size'], df['rbtree_ms'], label='Red-Black Tree (O(log n))', marker='^', linewidth=2, markersize=4)
plt.plot(df['size'], df['hash_ms'], label='Hash Table (O(1) avg)', marker='d', linewidth=2, markersize=4)
plt.plot(df['size'], df['multimap_ms'], label='std::multimap', marker='*', linewidth=2, markersize=4)

plt.xscale('log')
plt.yscale('log')
plt.xlabel('Размер массива N', fontsize=12)
plt.ylabel('Время поиска, ms', fontsize=12)
plt.title('Сравнение времени поиска всех записей по ФИО жениха', fontsize=14, fontweight='bold')
plt.legend(loc='upper left', fontsize=10)
plt.grid(True, alpha=0.3, linestyle='--')

# График 2: Коллизии хеш-функции
plt.subplot(2, 1, 2)
plt.plot(df['size'], df['collision_rate'] * 100, marker='o', color='red', linewidth=2, markersize=6)
plt.xscale('log')
plt.xlabel('Размер массива N', fontsize=12)
plt.ylabel('Коэффициент коллизий (%)', fontsize=12)
plt.title('Зависимость коллизий хеш-функции от размера данных', fontsize=14, fontweight='bold')
plt.grid(True, alpha=0.3, linestyle='--')

plt.tight_layout()
plt.savefig('search_comparison.png', dpi=150, bbox_inches='tight')
plt.show()

# Вывод статистики
print("СТАТИСТИКА ПО ВСЕМ РАЗМЕРАМ ДАННЫХ")
print(f"{'Метод':<20} {'Среднее время (ms)':<20} {'Станд. отклонение':<20}")
for method in ['linear_ms', 'bintree_ms', 'rbtree_ms', 'hash_ms', 'multimap_ms']:
    name = method.replace('_ms', '').replace('_', ' ').title()
    mean_val = df[method].mean()
    std_val = df[method].std()
    print(f"{name:<20} {mean_val:<20.4f} {std_val:<20.4f}")
print(f"Средний коэффициент коллизий: {df['collision_rate'].mean()*100:.2f}%")
print(f"Максимальный коэффициент коллизий: {df['collision_rate'].max()*100:.2f}%")
