//
// Created by Thinkman on 02/02/2018.
//
#include "ThinkPtrArray.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "utils.h"

static void think_ptr_array_maybe_expand(ThinkPtrArray* pArray);

ThinkPtrArray* think_ptr_array_new(ThinkDestoryFunc pDestroyFunc)
{
    ThinkPtrArray* pArray = (ThinkPtrArray*) malloc(sizeof(ThinkPtrArray));
    if (NULL == pArray) {
        return NULL;
    }

    pArray->m_nLen = 0;

    unsigned int nLen = sizeof(void *) * 2;

    pArray->m_nArySize = nLen;
    pArray->m_pData = malloc(nLen);

    if (NULL == pArray->m_pData) {
        goto err_ret;
    }

    pArray->m_pDestoryFunc = pDestroyFunc;

    return pArray;

err_ret:
    free(pArray);
    pArray = NULL;

    return NULL;
}

void think_ptr_array_free(ThinkPtrArray** pArray)
{
    return_if_fail(NULL != pArray);

    for (int i = 0; i < (*pArray)->m_nLen; ++i) {
        if ((*pArray)->m_pDestoryFunc != NULL) {
            (*((*pArray)->m_pDestoryFunc))((*pArray)->m_pData[i]);
        }
    }

    free((*pArray)->m_pData);
    *pArray = NULL;
}

static void think_ptr_array_maybe_expand(ThinkPtrArray* pArray)
{
    return_if_fail(NULL != pArray);

    if (pArray->m_nLen * sizeof(void*) <= pArray->m_nArySize / 2) {
        return;
    }

    pArray->m_pData = realloc(pArray->m_pData, pArray->m_nArySize * 2);
    pArray->m_nArySize = pArray->m_nArySize * 2;
}

unsigned int think_ptr_array_length(ThinkPtrArray *pArray)
{
    return_val_if_fail(NULL != pArray, 0);

    return pArray->m_nLen;
}

void think_ptr_array_append(ThinkPtrArray* pArray, void* pData)
{
    return_if_fail(NULL != pArray);

    pArray->m_pData[pArray->m_nLen++] = pData;
    think_ptr_array_maybe_expand(pArray);
}

void think_ptr_array_insert_at(ThinkPtrArray* pArray, void* pData, unsigned int nIndex)
{
    return_if_fail(NULL != pArray);

    if (nIndex > pArray->m_nLen) {
        return;
    }

    for (unsigned int i = pArray->m_nLen; i > nIndex; --i) {
        pArray->m_pData[i] = pArray->m_pData[i - 1];
    }

    pArray->m_pData[nIndex] = pData;
    pArray->m_nLen += 1;

    think_ptr_array_maybe_expand(pArray);
}

void think_ptr_array_prepend(ThinkPtrArray* pArray, void* pData)
{
    return_if_fail(NULL != pArray);

    for (int i = pArray->m_nLen; i > 0; --i) {
        pArray->m_pData[i] = pArray->m_pData[i - 1];
    }

    pArray->m_pData[0] = pData;
    pArray->m_nLen += 1;

    think_ptr_array_maybe_expand(pArray);
}

void think_ptr_array_remove(ThinkPtrArray* pArray, void* pData, int bFree)
{
    return_if_fail(NULL != pArray);

    if (0 == pArray->m_nLen) {
        return;
    }

    for (int i = 0; i < pArray->m_nLen; ++i) {
        if (pData == pArray->m_pData[i]) {
            think_ptr_array_remove_at(pArray, i, bFree);
            break;
        }
    }
}

void think_ptr_array_remove_at(ThinkPtrArray* pArray, unsigned int nIndex, int bFree)
{
    return_if_fail(NULL != pArray);

    if (0 == pArray->m_nLen) {
        return;
    }

    if (nIndex >= pArray->m_nLen) {
        return;
    }

    void* pData = pArray->m_pData[nIndex];
    for (int i = nIndex + 1; i < pArray->m_nLen; ++i) {
        pArray->m_pData[i - 1] = pArray->m_pData[i];
    }

    if (bFree && pArray->m_pDestoryFunc != NULL) {
        (*(pArray->m_pDestoryFunc))(pData);
    }

    pArray->m_nLen -= 1;
}

void* think_ptr_array_get_array_index(ThinkPtrArray* pArray, unsigned int nIndex)
{
    return_val_if_fail(NULL != pArray, NULL);

    if (nIndex >= pArray->m_nLen) {
        return NULL;
    }

    return pArray->m_pData[nIndex];
}

void* think_ptr_array_pop(ThinkPtrArray* pArray)
{
    return_val_if_fail(NULL != pArray, NULL);

    if (0 == pArray->m_nLen) {
        return NULL;
    }

    void* pData = pArray->m_pData[0];
    for (int i = 1; i < pArray->m_nLen; ++i) {
        pArray->m_pData[i - 1] = pArray->m_pData[i];
    }

    pArray->m_nLen -= 1;

    return pData;
}

void* think_ptr_array_pop_tail(ThinkPtrArray* pArray)
{
    return_val_if_fail(NULL != pArray, NULL);

    if (0 == pArray->m_nLen) {
        return NULL;
    }

    void* pData = pArray->m_pData[pArray->m_nLen - 1];
    pArray->m_nLen -= 1;

    return pData;
}

