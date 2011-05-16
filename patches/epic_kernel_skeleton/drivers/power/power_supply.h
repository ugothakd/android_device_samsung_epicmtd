/*
 *  Functions private to power supply class
 *
 *  Copyright © 2007  Anton Vorontsov <cbou@mail.ru>
 *  Copyright © 2004  Szabolcs Gyurko
 *  Copyright © 2003  Ian Molton <spyro@f2s.com>
 *
 *  Modified: 2004, Oct     Szabolcs Gyurko
 *
 *  You may use this code as per GPL version 2
 */


extern int power_supply_create_attrs(struct power_supply *psy);
extern void power_supply_remove_attrs(struct power_supply *psy);
extern int power_supply_uevent(struct device *dev, struct kobj_uevent_env *env);



extern void power_supply_update_leds(struct power_supply *psy);
extern int power_supply_create_triggers(struct power_supply *psy);
extern void power_supply_remove_triggers(struct power_supply *psy);

