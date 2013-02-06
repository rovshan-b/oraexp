select sn.name, ms.value from v$mystat ms inner join v$statname sn on ms.statistic# = sn.statistic#
order by (case when value = 0 then 1 else 0 end), sn.name
