import pandas as pd
import os

def game_sort_key_impl(game_value):
    tie_breaker = game_value % 1
    if game_value < 31:
        return 0 + tie_breaker
    if int(game_value) == 31:
        return 10 + tie_breaker
    if int(game_value) == 32:
        return 9 + tie_breaker
    if int(game_value) == 40:
        return 8 + tie_breaker
    if int(game_value) == 37:
        return 7 + tie_breaker
    if int(game_value) == 36:
        return 6 + tie_breaker
    if int(game_value) == 35:
        return 5 + tie_breaker
    if int(game_value) == 34:
        return 4 + tie_breaker
    if int(game_value) == 33:
        return 3 + tie_breaker
    
def game_sort_key(arr):
    return [game_sort_key_impl(value) for value in arr]

if __name__ =="__main__":
    source_filename = "data_600000_samples"
    data = pd.read_csv(f"raw_data/{source_filename}.csv")

    print(data.head())
    print(data)

    grande = data[['Mano', 'Grande', 'Stddev_grande', 'Grande_value']].sort_values('Grande_value', ascending=False)
    print(grande)

    chica = data[['Mano', 'Chica', 'Stddev_chica', 'Chica_value']].sort_values('Chica_value', ascending=True)
    print(chica)

    pares = data[['Pares_repr', 'Pares_1', 'Stddev_pares_1', 'Pares_2', 'Stddev_pares_2', 'Pares_value']].query("`Pares_value` > 1").groupby('Pares_repr', as_index=False).mean().sort_values('Pares_value', ascending=False)
    print(pares)

    juego = data[['Juego_repr', 'Juego_1', 'Stddev_juego_1', 'Juego_2', 'Stddev_juego_2', 'Juego_value']].query("`Juego_value` >= 31").groupby('Juego_repr', as_index=False).mean().sort_values('Juego_value', key=game_sort_key, ascending=False)
    print(juego)
    
    punto = data[['Juego_repr', 'Punto', 'Stddev_punto', 'Juego_value']].query("`Juego_value` < 31").groupby('Juego_repr', as_index=False).mean().sort_values('Juego_value', ascending=False).rename(columns={"Juego_repr": "Punto_repr", "Juego_value": "Punto_value"})
    print(punto)

    juego_con_pares = data[['Juego_repr', 'Juego_1', 'Stddev_juego_1', 'Juego_2', 'Stddev_juego_2', 'Juego_value', 'Pares_value']].query("`Juego_value` >= 31 and `Pares_value` > 1").groupby('Juego_repr', as_index=False).mean().sort_values('Juego_value', key=game_sort_key, ascending=False)
    print(juego_con_pares)
    
    juego_sin_pares = data[['Juego_repr', 'Juego_1', 'Stddev_juego_1', 'Juego_2', 'Stddev_juego_2', 'Juego_value', 'Pares_value']].query("`Juego_value` >= 31 and `Pares_value` < 1").groupby('Juego_repr', as_index=False).mean().sort_values('Juego_value', key=game_sort_key, ascending=False)
    print(juego_sin_pares)

    j = pd.read_csv("raw_data_outaded/juego.csv")
    j_s_p = pd.read_csv("raw_data_outaded/juego_sin_pares.csv")
    j_c_p = pd.read_csv("raw_data_outaded/juego_con_pares.csv")
    
    p = pd.read_csv("raw_data_outaded/punto.csv")
    p_s_p = pd.read_csv("raw_data_outaded/punto_sin_pares.csv")
    p_c_p = pd.read_csv("raw_data_outaded/punto_con_pares.csv")

    juego_extra_data_combined = j.merge(j_s_p, on='juego', how='outer', suffixes=('_general', '_sin_pares')).merge(j_c_p, on='juego', how='outer').rename(columns={"probabilidad": "probabilidad_con_pares"}).sort_values('juego', key=game_sort_key, ascending=False)
    print(juego_extra_data_combined)
    
    juego_combined = juego.merge(juego_sin_pares, on='Juego_repr', how='outer', suffixes=('_general', '_sin_pares')).merge(juego_con_pares, on='Juego_repr', how='outer').rename(columns={
        "Juego_1": "Juego_1_con_pares",
        "Stddev_juego_1": "Stddev_juego_1_con_pares",
        "Juego_2": "Juego_2_con_pares",
        "Stddev_juego_2": "Stddev_juego_2_con_pares",    
    }).sort_values('Juego_value', key=game_sort_key, ascending=False)
    print(juego_extra_data_combined)
    
    punto_extra_data_combined = p.merge(p_s_p, on='punto', how='outer', suffixes=('_general', '_sin_pares')).merge(p_c_p, on='punto', how='outer').rename(columns={"probabilidad": "probabilidad_con_pares"}).sort_values('punto', ascending=False)
    print(punto_extra_data_combined)
    
    assert(round(sum(p['probabilidad'])) == 100)
    assert(round(sum(p_s_p['probabilidad'])) == 100)
    assert(round(sum(p_c_p['probabilidad'])) == 100)
    assert(round(sum(j['probabilidad'])) == 100)
    assert(round(sum(j_s_p['probabilidad'])) == 100)
    assert(round(sum(j_c_p['probabilidad'])) == 100)
    print(pares['Pares_value'].nunique(), pares['Pares_repr'].nunique())
    assert(pares['Pares_value'].nunique() == pares['Pares_repr'].nunique())
    assert(pares['Pares_value'].nunique() == 208)

    os.makedirs(f'processed_data/{source_filename}', exist_ok=True)  
    data.to_csv(f'processed_data/{source_filename}/0___raw_data.csv')
    grande.to_csv(f'processed_data/{source_filename}/1_grande.csv')
    chica.to_csv(f'processed_data/{source_filename}/2_chica.csv')
    pares.to_csv(f'processed_data/{source_filename}/3_pares.csv')
    juego_combined.to_csv(f'processed_data/{source_filename}/4_juego.csv')
    juego_extra_data_combined.to_csv(f'processed_data/{source_filename}/5_juego_extra_data.csv')
    punto.to_csv(f'processed_data/{source_filename}/6_punto.csv')
    punto_extra_data_combined.to_csv(f'processed_data/{source_filename}/7_punto_extra_data.csv')

    print("Here")

    pass