/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
	for (int i = 0; i < HT_SIZE; i++)
	{
		(*table)[i] = NULL;
	}
	
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
	int hash = get_hash(key); // Transform key to table hash index.
	ht_item_t *item = (*table)[hash]; // Save the bucket pointer.

	if (item == NULL)
	{
		return NULL;
	}

	while (item != NULL) {
        if (strcmp(item->key, key)) { // stings are different.
			item = item->next; // Go to next item in the chain.
        } else { // strcmp returns '0' if same strings.
			return item;
		}
    }
    return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
	int hash = get_hash(key); // Transform key to table hash index.
	ht_item_t *item = (*table)[hash]; // Save the bucket pointer.

	if (item != NULL) { // Bucket is not empty.
        item->value = value; // Replace the value.
        return;
    }

	ht_item_t *insert_item = (ht_item_t *) malloc(sizeof(ht_item_t));
	if (insert_item == NULL) // Allocation faild.
	{
		return;
	}

	insert_item->key = key;
	insert_item->value = value;
	insert_item->next = (*table)[hash];
	(*table)[hash] = insert_item;
	}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
	ht_item_t *item = ht_search(table, key);
	if (item != NULL) {
		return &(item->value);
	}

    return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
	int hash = get_hash(key); // Transform key to table hash index.
	ht_item_t *current_item = (*table)[hash]; // Save the bucket pointer.
	if (current_item == NULL) // Bucket is empty.
	{
		return;
	}
	

	ht_item_t *pre_item = NULL;
	while (current_item != NULL) {

		if (strcmp(current_item->key, key)) { // Keys are different.
			pre_item = current_item;
			current_item = current_item->next; // Go to the next item in the chain.

		} else { // Keys are the same.

			if (pre_item == NULL) { // First item in the chain.
				(*table)[hash] = current_item->next; // Set the bucket pointer to the next item.

			} else { // Not first item in the chain.
				pre_item->next = current_item->next;
			}
			free(current_item);
			return;
		}
	}
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
	ht_item_t *current_item = NULL;
	ht_item_t *next_item = NULL;

	for (int key = 0; key < HT_SIZE; key++) {
		current_item = (*table)[key];
		
		while (current_item != NULL) {
			next_item = current_item->next;
			free(current_item);
			current_item = next_item;
		}
		(*table)[key] = NULL;
	}
}
