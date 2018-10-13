package waterhole.miner.eth;

public interface StateObserver {

    /**
     * 开始连接矿池.
     */
    void onConnectPoolBegin();

    /**
     * 连接矿池成功.
     */
    void onConnectPoolSuccess();

    /**
     * 连接矿池失败.
     *
     * @param error 错误信息
     */
    void onConnectPoolFail(String error);

    /**
     * 与矿池连接断开.
     *
     * @param error 错误信息
     */
    void onPoolDisconnect(String error);

    /**
     * 矿池推送的数据.
     *
     * @param message 下发的消息
     */
    void onMessageFromPool(String message);

    /**
     * 挖矿中产生异常.
     *
     * @param error 错误信息
     */
    void onMiningError(String error);

    /**
     * 挖矿进度回调.
     *
     * @param speed 挖矿速度
     */
    void onMiningStatus(double speed);
}
