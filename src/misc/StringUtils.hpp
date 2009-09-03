#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <string>
#include <cstdarg>

/**
 * Construire une std::string formatée "à la printf"
 * @param[in] format: formatage de la chaîne
 * @param[in] ...: arguments
 * @return string formée
 */
std::string str_sprintf(const char format[], ...);
std::wstring str_sprintf(const wchar_t format[], ...);

/**
 * Remplacer un motif par un autre
 * @param[in, out] target: chaîne cible
 * @param[in] look_for: sous-chaîne recherchée
 * @param[in] replace_by: sous-chaîne à caser
 * @return nombre d'occurences remplacées
 */
int str_replace(std::string& target, const std::string& look_for, const std::string& replace_by);
int str_replace(std::string& target, char look_for, char replace_by);

/**
 * Supprimer les blancs en début et fin de chaîne
 * @param[in] str: chaînée à tronquer
 * @return chaîne tronquée
 */
std::string str_trim(const std::string& str);

/**
 * Extraire une sous-chaîne
 * @param[in] str: chaîne cible
 * @param[in] from: indice de début d'extraction
 * @param[in] to: indice de fin d'extraction
 * @return chaîne extraite
 * @pre from < to
 */
std::string str_extract(const std::string& str, int from, int to);

#endif // STRINGUTILS_HPP
