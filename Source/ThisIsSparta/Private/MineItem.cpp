// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"

AMineItem::AMineItem()
{
	ItemType = "Mine";
	ExplosionDelay = 3.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 40.0f;
}

void AMineItem::ActivateItem(AActor* Activator)
{
	DestroyItem();
}
